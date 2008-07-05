#include "networkpm.h"
/*
 * fills dp with data. returns true as long as there is more data
 */
bool networkpm::dumpdata(datapacket& dp)
{
	char b1[16], b2[16];
	bool hasroom=true;
	SDL_mutexP(packetlock);
	pmdict::iterator i = packets.begin();
	while(i != packets.end())
	{
		if (!(hasroom = dp.adddata((*i).first.src, (*i).first.dst, (*i).first.color, (*i).second)))
			break;
		packets.erase(i++);
		count--;
	}
	SDL_mutexV(packetlock);
	return !hasroom;
}
