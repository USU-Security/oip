
#include "clientmanager.h"
#include <iostream>
using std::cerr;

//saves the args, and starts up the listening thread. 
clientmanager::clientmanager(connectioncallback cc, unsigned short port)
:ccallback(cc), listenport(port), running(true)
{
	thread = SDL_CreateThread(serverthread, this);
	if(-1 == SDLNet_Init())
		cerr << "Unable to initialize the net sublayer of SDL: " << SDLNet_GetError() << "\n";
}

//shuts everything down
clientmanager::~clientmanager()
{
	running = false;
	SDL_WaitThread(thread, NULL);
	SDLNet_Quit();
}

int clientmanager::serverthread(void* d)
{
	clientmanager* self = (clientmanager*)d;
	UDPsocket listen = SDLNet_UDP_Open(self->listenport);
	UDPpacket* p = SDLNet_AllocPacket(MAXPACKET);
	while (self->running)
	{
		Uint32 now = SDL_GetTicks();
		vector<client*>::iterator i;
		if (SDLNet_UDP_Recv(listen, p) == 1)
		{
			map<string, string> m;
			packet incoming(p->data, p->len);
			switch (incoming.gettype())
			{
			case SETUP:
				{
					setuppacket sp(p->data, p->len);
					sp.getopts(m);
					self->datasets.push_back(new client(p->address, SDL_GetTicks(), sp.getid()));
					self->ccallback(self->datasets.back()->dataset, m);
				}
				break;
			case SENDDATA:
				for (i = self->datasets.begin(); i != self->datasets.end(); ++i)
				{
					if (p->address.host == (*i)->ip.host && p->address.port == (*i)->ip.port && (*i)->sid == incoming.getid())
					{
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
					if (p->address.host == (*i)->ip.host && p->address.port == (*i)->ip.port && (*i)->sid == incoming.getid())
					{
						(*i)->lastsent = 0;
						(*i)->dataset.consumerdead();
						break;
					}
				}
				break;
			default:
				cerr << "Unknown message type: " << incoming.gettype() << "\n";
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
				continue;
			}
			//should the stream time out? TODO: make this 10 second value a config option
			else if ((*i)->lastsent + 10000  < now)
				(*i)->dataset.consumerdead();
			else if ((*i)->lastsent + MINRATE < now) //has the minimum time passed?
			{
				//is there any data?
				if ((*i)->dataset.size())
					drain = true;
			}
			else if ((*i)->dataset.size() >= datapacket::MAXDATA) //is there too much data in the buffer?
				drain = true;
			if (drain) //do we need to drain the buffer?
			{
				p->address = (*i)->ip;
				datapacket dp(p->data);
				dp.setid((*i)->sid);
				bool moredata;
				do
				{
					moredata = (*i)->dataset.dumpdata(dp);
					p->len = dp.getsize();
					SDLNet_UDP_Send(listen, -1, p);
				}
				while (moredata);
			}

			//we don't want to increment if we deleted it, so it goes here
			++i;				
		}
	}
	SDLNet_FreePacket(p);
	SDLNet_UDP_Close(listen);
}



