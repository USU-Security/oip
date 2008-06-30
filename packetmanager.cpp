
#include "packetmanager.h"
#include <arpa/inet.h>

bool packetmanager::addpacket(unsigned int s, unsigned int d, unsigned int c, unsigned int size)
{
	SDL_mutexP(packetlock);
	packettype p(s,d,c);
	map<packettype, unsigned int>::iterator i = packets.find(p);
	if (i == packets.end())
		packets[p] = size;
	else
		(*i).second += size;
	SDL_mutexV(packetlock);
	return true;
}

void packetmanager::dumppackets(particlemanager& pm)
{
	SDL_mutexP(packetlock);
	map<packettype, unsigned int>::iterator i = packets.begin();
	for (;i != packets.end(); ++i)
	{
		char srcip[16];
		char dstip[16];
        unsigned int sip = ntohl((*i).first.src);
        unsigned int dip = ntohl((*i).first.dst);
        snprintf(srcip, 16, "%d.%d.%d.%d", sip >> 24, (sip >> 16)&255, (sip >> 8)&255, sip & 255);
        snprintf(dstip, 16, "%d.%d.%d.%d", dip >> 24, (dip >> 16)&255, (dip >> 8)&255, dip & 255);
        pm.packet(string(srcip), string(dstip), (*i).second/2800.0, (*i).first.color);
	}
	packets.clear();
	SDL_mutexV(packetlock);
}


