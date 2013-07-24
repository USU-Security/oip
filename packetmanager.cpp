/*
	Copyright 2008 Utah State University    

	This file is part of OIP.

    OIP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OIP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OIP.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "packetmanager.h"
#include "ips.h"
#include <iostream>
using std::cerr;

bool packetmanager::addpacket(unsigned int s, unsigned int d, unsigned int c, unsigned int size)
{
	if (count < maxpacket)
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
	}
	else
	{
		cerr << "Too many packets in the queue, dropping\n";
	}
	return true;
}

/*
 * dumps all the accumulated packets into a packetsink object
 */
//bool packetmanager::dumpdata(packetsink& ps)
bool packetmanager::dumpdata(packetsink& ps, int speed)
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

