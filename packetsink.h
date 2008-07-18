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
