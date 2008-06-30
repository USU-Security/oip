/*
 * this class will handle packets recieved from libpcap, and store them in easy to manage ways
 */


#ifndef PACKETMANAGER_H
#define PACKETMANAGER_H

#include<map>
using std::map;

#include "particlemanager.h"

class packettype
{
public:
	unsigned int src;
	unsigned int dst;
	unsigned int color;

	bool operator<(const packettype& o) const {return src < o.src || dst < o.dst || color < o.color;}
	packettype():src(0),dst(0),color(0) {}
	packettype(unsigned int s, unsigned int d, unsigned int c):src(s),dst(d),color(c) {}
};

class packetmanager
{
protected:
	map<packettype, unsigned int> packets;
	SDL_mutex* packetlock;
public:
	packetmanager() {packetlock = SDL_CreateMutex();}
	~packetmanager() {SDL_DestroyMutex(packetlock);}

	bool addpacket(unsigned int s, unsigned int d, unsigned int c, unsigned int size);
	void dumppackets(particlemanager& pm); 
	int size() {return packets.size();}
};
#endif //PACKETMANAGER_H

