/*
	Copyright 2008 Utah State University    

	This file is part of OIP.

    OIP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OIP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OIP.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "namecache.h"
#ifndef WIN32
#include <sys/socket.h>
#include <netdb.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#include "ips.h"
#include <iostream>
using std::cerr;
namecache names;
const string & namecache::operator[](unsigned int a)
{
	namehash::iterator i;
	i = names.find(a);
	if (i != names.end())
		return (*i).second;
	char host[256];
	//set a temporary hostname of the ip address as a string
	names[a] = longtoip(host, 256, ntohl(a));
	//trigger the lookup
	dolookup(a);
	//give up the cpu for a moment in the hope that it will succeed quickly
	SDL_Delay(1);
	return names[a];	
}


int namecache::cthread(void* s)
{
	namecache* self = (namecache*)s;
	SDL_mutexP(self->queuemutex);
	while (self->running)
	{
		if (self->queue.size() == 0) // no data to process
		{
			//wait until there is data
			self->lookupwait=true;
			SDL_CondWait(self->queuedata, self->queuemutex);
			self->lookupwait=false;
		}
		else
		{
			//	remove data from queue and process it
			unsigned int ip = self->queue.front();
			self->queue.pop_front();
			SDL_mutexV(self->queuemutex);
			//look up the ip
			struct sockaddr_in addr;
			addr.sin_family = PF_INET;
			addr.sin_addr.s_addr = ip;
			char host[256];
			char serv[256];
#ifndef WIN32
			if (getnameinfo((struct sockaddr*)&addr, sizeof(addr), host, 256, serv, 256, NI_NOFQDN))
				cerr << "Unable to look up " << longtoip(host, 256, ntohl(ip)) << "\n";
			else
			{
#else
			struct hostent* he;
			if (!(he = gethostbyaddr((char*)&addr.sin_addr, sizeof(addr.sin_addr), AF_INET)))
				cerr << "Unable to look up " << longtoip(host, 256, ntohl(ip)) << ": " << WSAGetLastError() <<"\n";
			else
			{
				strncpy(host, he->h_name, 256);
#endif	
				self->names[ip] = host;
			}
			SDL_mutexP(self->queuemutex);
		}
	}
	SDL_mutexV(self->queuemutex);//unlock mutex
	self->dead = true;
	return 0;
}
void namecache::dolookup(unsigned int a)
{
	SDL_mutexP(queuemutex);
	queue.push_back(a);
	if (lookupwait)
		SDL_CondSignal(queuedata);
	SDL_mutexV(queuemutex);
}

namecache::namecache()
:lookupwait(false),running(true),dead(false)
{
	queuemutex = SDL_CreateMutex();
	queuedata = SDL_CreateCond();
	tid = SDL_CreateThread(cthread, this);
}

namecache::~namecache()
{
	running = false;
	//try to get the thread to quit
	if (lookupwait)
		SDL_CondSignal(queuedata);
	SDL_Delay(10);
	//otherwise, kill it if it didnt quit
	if (!dead)
		SDL_KillThread(tid);
	else
		SDL_WaitThread(tid, NULL); //reap it
	if (queuedata)
		SDL_DestroyCond(queuedata);
	if (queuemutex)
		SDL_DestroyMutex(queuemutex);
}



