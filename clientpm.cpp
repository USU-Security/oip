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
#include "clientpm.h"
#include <iostream>
using std::cerr;

#ifndef WIN32
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#else
#include <winsock2.h>
#endif

#include "encrypt.h"

int clientpm::clientthread(void* s)
{
	clientpm* self = (clientpm*)s;
	Uint32 lastsent = SDL_GetTicks();
	Uint32 lastrecieved= SDL_GetTicks();
	int len;
	fd_set lset;
	FD_ZERO(&lset);
	struct timeval timeout;
	while(self->running)
	{
		//server gives us ~20 seconds to keep stream alive. respond every 2
		if (lastsent + 2000 < SDL_GetTicks() || lastsent > SDL_GetTicks())
		{
			senddata sd(self->data);
			aes.encrypt(self->data, sd.paddedsize());
			sendto(self->sock, (char*)self->data, sd.paddedsize(), 0, (struct sockaddr*)self->res->ai_addr, self->res->ai_addrlen);
			lastsent = SDL_GetTicks();
		}
		if (lastrecieved + 30000 < SDL_GetTicks()) //TODO: not sure how to handle overflow case
		{
			self->running = false;
			cerr << "Server hasn't responded in at least 30 seconds\n";
			break;
		} 
		FD_SET(self->sock, &lset);
		timeout.tv_sec = 0;
		timeout.tv_usec = MINRATE * 1000;
		if (select(self->sock+1, &lset, NULL, NULL, &timeout))
		{
			//very possible that a context switch happened during the select. check the running flag.
			if (!self->running)
				break;
			struct sockaddr_in inaddr;
#ifndef WIN32
			socklen_t inlen;
#else
			int inlen;
#endif
			inlen = sizeof(inaddr);

			len = recvfrom(self->sock, (char*)self->data, MAXPACKET, 0, (struct sockaddr*)&inaddr, &inlen);
			aes.decrypt(self->data, len);
			packet p(self->data, len);
			if (p.getid() == self->sid) 
			{
				if (p.gettype() == DUMPPACKETS)
				{
					//i keep dying in here due to a race condition. 
					datapacket dp(self->data, len);
					dp.dumpdata(*self);
				}
				lastrecieved = SDL_GetTicks();
			}
			else
			{
				cerr << "Invalid address or bad streamid\n";
			}
		}
	}
	return 0;
}

clientpm::clientpm(const string& server, const map<string, string> & opts, Uint16 port)
{
	running = true;
	struct addrinfo hints;
	res = NULL;
	hints.ai_family=PF_INET;
	hints.ai_socktype=SOCK_DGRAM;
	hints.ai_protocol=0;
#ifndef WIN32 //vc++6 doesnt have getaddrinfo
	hints.ai_flags = AI_NUMERICSERV;
	char ap[8];
	snprintf(ap, 8, "%hd", port);
	int result;
	if (result = getaddrinfo(server.c_str(), ap, &hints, &res)) 
	{
		cerr << "Unable to resolve " << server << ": " << gai_strerror(result) <<  "\n";
#else
	hostent* he;
	if (he=gethostbyname(server.c_str()))
	{
		res = &realres;
		res->ai_addr = &sin;
		res->ai_addr->sin_family = hints.ai_family;
		res->ai_addr->sin_port = htons(port);
		res->ai_addrlen = sizeof(realres);
		memcpy(&res->ai_addr->sin_addr, he->h_addr_list[0], he->h_length);
	}
	else
	{
		cout << "Unable to look up " << server << ": " << WSAGetLastError() << "\n";
#endif
		running = false;
		return;
	}

	
	
	
	sid = rand();

	//open the socket
	sock = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);

	//send the initial request.
	setuppacket sp(data);
	sp.setid(sid);
	sp.setopts(opts);
	aes.encrypt(data, sp.paddedsize());
	if (!sendto(sock, (char*)data, sp.paddedsize(), 0, (struct sockaddr*)res->ai_addr, res->ai_addrlen))
	{
		running = false;
		cerr << "Unable to send the stream setup message\n";
	}
	if (running)	
		thread = SDL_CreateThread(clientthread, this);
}

clientpm::~clientpm()
{
	//signal the thread to quit
	running = false;
	//wait for the thread to quit
	cerr << "Waiting for child thread...\n";
	SDL_WaitThread(thread, NULL);	
	//cerr << "Killing the thread\n";
	//SDL_KillThread(thread);
	cerr << "Freeing Resources for thread\n";
	//send an enddata message
	enddata ed(data);
	ed.setid(sid);
	aes.encrypt(data, ed.paddedsize());
	if (res)
		sendto(sock, (char*)data, ed.paddedsize(), 0, (struct sockaddr*)res->ai_addr, res->ai_addrlen);
#ifndef WIN32
	close(sock);
	if (res)
		freeaddrinfo(res);
#endif
}




