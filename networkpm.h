
#ifndef NETWORKPM_H
#define NETWORKPM_H

#include "packetmanager.h"
#include "messages.h"

//this class reimplements the packetmanager so that we can dump our particles
//to a datapacket instead
class networkpm: public packetmanager
{
private:
	bool consumer,producer;
public:
	networkpm():packetmanager(),consumer(true),producer(true)  {}
	bool addpacket(Uint32 s, Uint32 d, Uint32 c, Uint32 size) {if (consumer) packetmanager::addpacket(s,d,c,size); return consumer;}
	bool dumpdata(datapacket& dp);

	void consumerdead() {consumer=false;}
	void producerdead() {producer=false;}
	bool killme() {return not (producer || consumer); }
};

#endif //NETWORKPM_H
