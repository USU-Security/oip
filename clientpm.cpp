#include "clientpm.h"
#include <iostream>
using std::cerr;

int clientpm::clientthread(void* s)
{
	clientpm* self = (clientpm*)s;
	Uint32 lastsent = SDL_GetTicks();
	Uint32 lastrecieved= SDL_GetTicks();
	while(self->running)
	{
		//server gives us ~10 seconds to keep stream alive. respond every five
		if (lastsent + 5000 < SDL_GetTicks() || lastsent > SDL_GetTicks())
		{
			senddata sd(self->p->data);
			self->p->len = sd.getsize();
			self->p->address = self->ip;
			SDLNet_UDP_Send(self->sock, -1, self->p);
			lastsent = SDL_GetTicks();
		}
		if (lastrecieved + 30000 < SDL_GetTicks()) //TODO: not sure how to handle overflow case
		{
			self->running = false;
			cerr << "Server hasn't responded in at least 30 seconds\n";
			break;
		} 
		if (SDLNet_UDP_Recv(self->sock, self->p) == 1)
		{
			packet p(self->p->data, self->p->len);
			if (self->p->address.host == self->ip.host && self->p->address.port == self->ip.port && p.getid() == self->sid) 
			{
				if (p.gettype() == DUMPPACKETS)
				{
					datapacket dp(self->p->data, self->p->len);
					dp.dumpdata(*self);
				}
				lastrecieved = SDL_GetTicks();
			}
			else
			{
				cerr << "Invalid address or bad streamid\n";
			}
		}
		else
		{
			//i would rather this instance of SDLNet_UDP_Recv were a blocking call. 
			//if there isnt data, yield the processor
			SDL_Delay(1);
		}
	}
	return 0;
}

clientpm::clientpm(const string& server, const map<string, string> & opts, Uint16 port)
{
	running = true;
	if (SDLNet_ResolveHost(&ip, server.c_str(), port))
	{
		running = false;
		cerr << "Unable to resolve " << server << ": " << SDLNet_GetError() <<  "\n";
		return;
	}
	//allocate the packet
	if (!(p = SDLNet_AllocPacket(MAXPACKET)))
	{
		running = false;
		cerr << "Unable to allocate a packet: " << SDLNet_GetError() << "\n";
		return;
	}
	sid = rand();

	//open the socket
	sock = SDLNet_UDP_Open(-1);

	//send the initial request.
	setuppacket sp(p->data);
	sp.setid(sid);
	sp.setopts(opts);
	p->len = sp.getsize();
	p->address = ip;
	if (!SDLNet_UDP_Send(sock, -1, p))
	{
		running = false;
		cerr << "Unable to send the stream setup message: " << SDLNet_GetError() << "\n";
	}
	
	thread = SDL_CreateThread(clientthread, this);
}

clientpm::~clientpm()
{
	running = false;
	SDL_WaitThread(thread, NULL);
	//send an enddata message
	enddata ed(p->data);
	ed.setid(sid);
	p->len = ed.getsize();
	p->address = ip;
	SDLNet_UDP_Send(sock, -1, p);
	SDLNet_UDP_Close(sock);
	SDLNet_FreePacket(p);
}




