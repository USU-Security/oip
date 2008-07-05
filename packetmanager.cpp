
#include "packetmanager.h"
#include <arpa/inet.h>
#include "ips.h"

bool packetmanager::addpacket(unsigned int s, unsigned int d, unsigned int c, unsigned int size)
{
	SDL_mutexP(packetlock);
	packettype p(s,d,c);
	pmdict::iterator i = packets.find(p);
	if (i == packets.end())
	{
		packets[p] = size;
		count++;
	}
	else
		(*i).second += size;
	SDL_mutexV(packetlock);
	return true;
}

void packetmanager::dumppackets(particlemanager& pm)
{
	SDL_mutexP(packetlock);
	pmdict::iterator i = packets.begin();
	for (;i != packets.end(); ++i)
	{
		char srcip[16];
		char dstip[16];
        unsigned int sip = ntohl((*i).first.src);
        unsigned int dip = ntohl((*i).first.dst);
		longtoip(srcip, 16, sip);
		longtoip(dstip, 16, dip);
        pm.packet(string(srcip), string(dstip), (*i).second/2800.0, (*i).first.color);
	}
	packets.clear();
	count = 0;
	SDL_mutexV(packetlock);
}


