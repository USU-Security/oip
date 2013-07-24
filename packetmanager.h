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
 * this class will handle packets recieved from libpcap, and store them in easy to manage ways
 */


#ifndef PACKETMANAGER_H
#define PACKETMANAGER_H
#include "particlemanager.h"
#include "packetsink.h"
#include "packetpeek.h"

#define DEFAULTMAX 2000



class packettype
{
public:
	unsigned int src;
	unsigned int dst;
	unsigned int color;

	bool operator<(const packettype& o) const {return src < o.src || dst < o.dst || color < o.color;}
	bool operator==(const packettype& o) const {return src == o.src && dst == o.dst && color == o.color;}
	packettype():src(0),dst(0),color(0) {}
	packettype(unsigned int s, unsigned int d, unsigned int c):src(s),dst(d),color(c) {}
};
#ifndef WIN32
#include<ext/hash_map>
class pthash
{
public:
	size_t operator()(const packettype& pt) const
	{
		return pt.src ^ ((pt.dst << 17) | (pt.dst >> 15)) ^ (pt.color);
	}
};
typedef __gnu_cxx::hash_map<packettype, unsigned int, pthash> pmdict;
#else
#include<map>
typedef std::map<packettype, int> pmdict;
#endif




class packetmanager
{
protected:
	pmdict packets;
	SDL_mutex* packetlock;
	int count;
	int maxpacket;
public:
	packetmanager():count(0),maxpacket(DEFAULTMAX) {packetlock = SDL_CreateMutex();}
	virtual ~packetmanager() {SDL_DestroyMutex(packetlock);}

	bool addpacket(unsigned int s, unsigned int d, unsigned int c, unsigned int size);
    //virtual bool dumpdata(packetsink& ); 
	virtual bool dumpdata(packetsink&, int); 
	void copydata(packetpeek& );
	int size() {return count;}
	bool empty() {return packets.begin() == packets.end();}
};

#endif //PACKETMANAGER_H

