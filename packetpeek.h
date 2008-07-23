/*
 * abstract class. which describes the interface needed to copy packets from a
 * packet manager
 */

#ifndef PACKETPEEK_H
#define PACKETPEEK_H

class packetpeek
{
public:
	/*
	 * recieves a packet, returns whether it wants to accept more
	 * Should be pure virtual, but vc++6 hates that. 
	 */
	virtual void copypacket(unsigned int src, unsigned int dst, unsigned int color, unsigned int size) {}
};

#endif //PACKETPEEK_H
