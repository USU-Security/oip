//this class manages the client side of a connection

#ifndef CLIENTPM_H
#define CLIENTPM_H

#include <SDL/SDL_net.h>
#include "packetmanager.h"
#include "messages.h"

class clientpm: public packetmanager
{
private:
	SDL_Thread* thread;
	bool running;
	IPaddress ip;
	UDPpacket * p;
	Uint16 sid;
	UDPsocket sock;

	static int clientthread(void*self);
public:
	clientpm(const string& server, const map<string, string> & opts, Uint16 port=12751);
	~clientpm();

	bool alive() {return running;}
};

#endif //CLIENTPM_H

