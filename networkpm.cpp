#include "networkpm.h"

/*
 * fills dp with data. returns true as long as there is more data
 */
bool networkpm::dumpdata(datapacket& dp)
{
	bool hasroom;
	SDL_mutexP(packetlock);
	map<packettype, unsigned int>::iterator i = packets.begin(), todelete;
	while(i != packets.end())
	{
		if (!(hasroom = dp.adddata((*i).first.src, (*i).first.dst, (*i).first.color, (*i).second)))
			break;
		packets.erase(i++);
	}

	SDL_mutexV(packetlock);
	return !hasroom;
}
