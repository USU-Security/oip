
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
        pm.packet(
			names[(*i).first.src], 
			names[(*i).first.dst], (*i).second/2800.0, (*i).first.color);
	}
	packets.clear();
	count = 0;
	SDL_mutexV(packetlock);
}


