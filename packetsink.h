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
	 */
	virtual bool addpacket(unsigned int src, unsigned int dst, unsigned int color, unsigned int size) = 0;
};

#endif //PACKETSINK_H
