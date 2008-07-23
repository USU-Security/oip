
#include "packetmanager.h"
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

/*
 * dumps all the accumulated packets into a packetsink object
 */
bool packetmanager::dumpdata(packetsink& ps)
{
    bool hasroom=true;
    SDL_mutexP(packetlock);
    pmdict::iterator i = packets.begin();
    while(i != packets.end())
    {
        if (!(hasroom = ps.addpacket((*i).first.src, (*i).first.dst, (*i).first.color, (*i).second)))
            break;
        packets.erase(i++);
        count--;
    }
    SDL_mutexV(packetlock);
    return !hasroom;
}

void packetmanager::copydata(packetpeek& ps)
{
    SDL_mutexP(packetlock);
    pmdict::iterator i = packets.begin();
    while(i != packets.end())
    {
        ps.copypacket((*i).first.src, (*i).first.dst, (*i).first.color, (*i).second);
        i++;
    }
    SDL_mutexV(packetlock);
}

