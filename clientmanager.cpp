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

#include "clientmanager.h"
#include <iostream>
using std::cerr;
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "encrypt.h"

#include <unistd.h>


//saves the args, and starts up the listening thread. 
clientmanager::clientmanager(connectioncallback cc, unsigned short port)
:ccallback(cc), listenport(port), running(true)
{
	thread = SDL_CreateThread(serverthread, this);
}

//shuts everything down
clientmanager::~clientmanager()
{
	running = false;
	SDL_WaitThread(thread, NULL);
}

int clientmanager::serverthread(void* d)
{
	clientmanager* self = (clientmanager*)d;
	int listen = socket(PF_INET, SOCK_DGRAM, 0); 
	unsigned char data[MAXPACKET];
	struct sockaddr_in addr;
	addr.sin_family = PF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(self->listenport);
	bind(listen, (struct sockaddr*)&addr, sizeof(addr));
	//set up the descriptor set and timeout for a ready check
	fd_set lset;
	FD_ZERO(&lset);
	struct timeval timeout;;

	while (self->running)
	{
		Uint32 now = SDL_GetTicks();
		vector<client*>::iterator i;
		FD_SET(listen, &lset);
		timeout.tv_sec = 0;
		timeout.tv_usec = MINRATE * 1000;
		if (select(listen+1, &lset, NULL, NULL, &timeout)) //is there a packet ready?
		{	
			int len;
			socklen_t addrlen = sizeof(addr);
			addr.sin_addr.s_addr = htonl(INADDR_ANY);
			addr.sin_port = htons(self->listenport);
			len = recvfrom(listen, data, MAXPACKET, 0, (struct sockaddr*)&addr, &addrlen);
			aes.decrypt(data, len);
		
			map<string, string> m;
			packet incoming(data, len);
			switch (incoming.gettype())
			{
			case SETUP:
				{
					cout << "Setting up a client\n";
					setuppacket sp(data, len);
					sp.getopts(m);
					self->datasets.push_back(new client(addr.sin_addr.s_addr,addr.sin_port, SDL_GetTicks(), sp.getid()));
					self->datasets.back()->lastsent = now;
					self->ccallback(self->datasets.back()->dataset, m);
				}
				break;
			case SENDDATA:
				cout << "Client refresh message\n";
				for (i = self->datasets.begin(); i != self->datasets.end(); ++i)
				{
					if (addr.sin_addr.s_addr == (*i)->ip && addr.sin_port == (*i)->port && (*i)->sid == incoming.getid())
					{
						//cout << "Client Refreshed\n";
						(*i)->lastsent = now;
						break;
					}
				}
				break;
			//case DUMPPACKETS:
			//	break;
			case ENDDATA:
				for (i = self->datasets.begin(); i != self->datasets.end(); ++i)
				{
					if (addr.sin_addr.s_addr == (*i)->ip && addr.sin_port == (*i)->port && (*i)->sid == incoming.getid())
					{
						(*i)->lastsent = 0;
						(*i)->dataset.consumerdead();
						break;
					}
				}
				break;
			default:
				cerr << "Unknown message type: " << (int)incoming.gettype() << "\n";
			}
			

		}
		//check to see if any of the streams need management
		for (i = self->datasets.begin(); i != self->datasets.end();)
		{
			bool drain = false;
			//check for clock overflow. normally not too worrisome, except that clients
			//could hang around for up to 50 days if we don't deal with it. 
			if ((*i)->lastsent > now) //did the clock roll over? 
				(*i)->lastsent = 0; //gives them a little time. 

			//should we remove this one?
			if ((*i)->dataset.killme())
			{
				delete(*i);
				*i = self->datasets.back();
				self->datasets.pop_back();
				cout << "Removed client\n";
				continue;
			}
			//should the stream time out? TODO: make this 10 second value a config option
			else if ((*i)->lastsent + 20000  < now)
			{
				(*i)->dataset.consumerdead();
				cout << "Stream timed out, producer: " << (*i)->dataset.producer << ", killme: " << (*i)->dataset.killme() << "\n";
			}
			else if ((*i)->lastsent + MINRATE < now) //has the minimum time passed?
			{
				//is there any data?
				if (!(*i)->dataset.empty())
					drain = true;
			}
			//this is giving me issues. 
			else if ((*i)->dataset.size() >= datapacket::MAXDATA) //is there too much data in the buffer?
				drain = true;
			if (drain) //do we need to drain the buffer?
			{
				addr.sin_addr.s_addr = (*i)->ip;
				addr.sin_port =(*i)->port;
				datapacket dp(data);
				dp.setid((*i)->sid);
				bool moredata;
				int speed;
				do
				{
					//moredata = (*i)->dataset.dumpdata(dp);
					moredata = (*i)->dataset.dumpdata(dp, speed);
					//encrypt the data 
					aes.encrypt(data, dp.paddedsize());
					sendto(listen, data, dp.paddedsize(), 0, (struct sockaddr*)&addr, sizeof(addr));
				}
				while (moredata);
			}

			//we don't want to increment if we deleted it, so it goes here
			++i;				
		}
	}
	cout << "Exiting server listening thread\n";
	close(listen);
}



