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
/*
 * abstract class. which describes the interface needed to recieve packets
 * from a packetmanager
 */

#ifndef PACKETSINK_H
#define PACKETSINK_H

class packetsink
{
public:
	/*
	 * recieves a packet, returns whether it wants to accept more
	 * Should be pure virtual, but vc++6 hates that. 
	 */
	virtual bool addpacket(unsigned int src, unsigned int dst, unsigned int color, unsigned int size) {return false;}
};

#endif //PACKETSINK_H
