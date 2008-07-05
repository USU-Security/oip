#include "clientpm.h"
#include <iostream>
using std::cerr;

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

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
		//server gives us ~10 seconds to keep stream alive. respond every five
		if (lastsent + 5000 < SDL_GetTicks() || lastsent > SDL_GetTicks())
		{
			senddata sd(self->data);
			sendto(self->sock, self->data, sd.getsize(), 0, self->res->ai_addr, self->res->ai_addrlen);
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
			struct sockaddr_in inaddr;
			socklen_t inlen = sizeof(inaddr);

			len = recvfrom(self->sock, self->data, MAXPACKET, 0, (struct sockaddr*)&inaddr, &inlen);
			packet p(self->data, len);
			if (p.getid() == self->sid) 
			{
				if (p.gettype() == DUMPPACKETS)
				{
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
	hints.ai_flags = AI_NUMERICSERV;
	char ap[8];
	snprintf(ap, 8, "%hd", port);
	int result;
	if (result = getaddrinfo(server.c_str(), ap, &hints, &res)) 
	{
		running = false;
		cerr << "Unable to resolve " << server << ": " << gai_strerror(result) <<  "\n";
		return;
	}
	
	
	
	sid = rand();

	//open the socket
	sock = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);

	//send the initial request.
	setuppacket sp(data);
	sp.setid(sid);
	sp.setopts(opts);
	if (!sendto(sock, data, sp.getsize(), 0, res->ai_addr, res->ai_addrlen))
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
	//send an enddata message
	enddata ed(data);
	ed.setid(sid);
	sendto(sock, data, ed.getsize(), 0, res->ai_addr, res->ai_addrlen);
	//wait for the thread to quit
	SDL_WaitThread(thread, NULL);	
	close(sock);
	if (res)
		freeaddrinfo(res);
}




