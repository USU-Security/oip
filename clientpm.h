//this class manages the client side of a connection

#ifndef CLIENTPM_H
#define CLIENTPM_H

#include "packetmanager.h"
#include "messages.h"

class clientpm: public packetmanager
{
private:
	SDL_Thread* thread;
	bool running;
	Uint32 ip;
	Uint16 port;
	unsigned char data[MAXPACKET];	
	Uint16 sid;
	Uint32 sock;
	struct addrinfo* res;

	static int clientthread(void*self);
public:
	clientpm(const string& server, const map<string, string> & opts, Uint16 port=12751);
	~clientpm();

	bool alive() {return running;}

	//not really necessary for functioning, but necessary to be able to do test
	pmdict::const_iterator begin() {return packets.begin();}
	pmdict::const_iterator end() {return packets.end();}
};

#endif //CLIENTPM_H

