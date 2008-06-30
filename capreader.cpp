/*
 * This file tests the entity manager
 */

#include <iostream>
using namespace std;

//#include <sys/time.h>
#include <SDL/SDL.h>
#include <stdlib.h>
#include "text.h"
#include "particlemanager.h"
#include "packetmanager.h"
#include <pcap.h>
#include "structs.h"
#include <arpa/inet.h>

#define frand ((float)rand()/RAND_MAX)
#define SNAPLEN 80
	
bool run=true;
particlemanager pm;
packetmanager packetlist;
pcap_t * handle;
SDL_Surface* screen;

inline double now()
{
	//struct timeval tv;
	//gettimeofday(&tv, NULL);
	//return tv.tv_sec + tv.tv_usec/1000000.0;
	return SDL_GetTicks()/1000.0;
}

SDL_Surface* resize(int w, int h)
{
	SDL_Surface * screen;
	if (!(screen = SDL_SetVideoMode(w,h,32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_RESIZABLE)))
	{
		cout << "Unable to get a hardware surface, falling back on software\n";
		screen = SDL_SetVideoMode(w,h,32, SDL_SWSURFACE|SDL_RESIZABLE);
	}
	if (!screen) 
	{
		cout << "Unable to set display: " << SDL_GetError() << "\n";
		return NULL;
	}
	return screen;
}

SDL_Surface* initsdl()
{
	if ( SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "Unable to initialize sdl: " << SDL_GetError() << "\n";
		return NULL;
	}
	atexit(SDL_Quit);
	return resize(512,512);
}

void hexprint(const unsigned char * b, int size)
{
	char tmp[10];
	while (size--)
	{
		snprintf(tmp, 10, "%02x ", (int)*b);
		b++;
	}
}

int sniff(void* a)
{
	struct pcap_pkthdr* header;
	const u_char* packet;
	const struct sniff_ethernet *ethernet;
	const struct sniff_ip *ip;
	int result;
	while(run)
	{
		if (1 == (result = pcap_next_ex(handle, &header, &packet)))
		{
			ethernet = (struct sniff_ethernet*)packet;
			if (ntohs(ethernet->ether_type) == T_IP) 
			{
				ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
				if (ip->ip_p == T_TCP) //could use header.len/1400.0 or something
					packetlist.addpacket(ip->ip_src, ip->ip_dst, 0xff00ff00, header->len);
				else if (ip->ip_p == T_UDP)
					packetlist.addpacket(ip->ip_src, ip->ip_dst, 0xffff0000, header->len);
				else
					packetlist.addpacket(ip->ip_src, ip->ip_dst, 0xff0000ff, header->len);
			}
		}
	}

}

int main(int argc, char** argv)
{
	char* dev, errbuf[PCAP_ERRBUF_SIZE];
	//get a device to read from
	if (argc == 1)
	{
		dev = pcap_lookupdev(errbuf);
		if (!dev)
		{
			cerr << "Unable to open default device: " << errbuf << endl;
			return 1;
		}
	}
	else
		dev = argv[1];

	//open up the device
	handle = pcap_open_live(dev, SNAPLEN, 1, 100, errbuf);
	if (!handle)
	{
		cerr << "Unable to open " << dev << ": " << errbuf << endl;
		return 1;
	}

	screen = initsdl();
	if (!screen)
		return 1;

	//start up the sniffing thread
	SDL_Thread * tid;
	tid = SDL_CreateThread(sniff, NULL);

	int i;
	double ti = now();
	while(run)
	{
	
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT: 
				run = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					run = false;
				break;
			case SDL_VIDEORESIZE:
				resize(event.resize.w, event.resize.h);
				break;
			case SDL_MOUSEBUTTONDOWN:
				pm.mousedown(event.button.x, event.button.y);
				break;
			case SDL_MOUSEMOTION:
				pm.mousemove(event.button.x, event.button.y);
				break;
			case SDL_MOUSEBUTTONUP:
				pm.mouseup();
				break;
				
			}
		}
		double dt = now() - ti;
		ti = now();
		packetlist.dumppackets(pm);
		pm.process(dt);
		SDL_FillRect(screen, NULL, 0);
		pm.draw(screen);
		char s[128];
		snprintf(s, 128, "%g fps", 1.0/dt);
		text.render(screen, s, 0, 0, 8);
		SDL_Flip(screen);
		//don't bother going faster than twice the minimum framerate
		if (dt * 1000 < MINRATE)
			SDL_Delay(MINRATE - (int)(dt * 1000));
	}

	return 0;
}

