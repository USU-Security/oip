/*
 * this class will handle packets recieved from libpcap, and store them in easy to manage ways
 */


#ifndef PACKETMANAGER_H
#define PACKETMANAGER_H
#include "particlemanager.h"
#include "packetsink.h"


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
public:
	packetmanager():count(0) {packetlock = SDL_CreateMutex();}
	~packetmanager() {SDL_DestroyMutex(packetlock);}

	bool addpacket(unsigned int s, unsigned int d, unsigned int c, unsigned int size);
	bool dumpdata(packetsink& ); 
	int size() {return count;}
	bool empty() {return packets.begin() == packets.end();}
};
#endif //PACKETMANAGER_H

