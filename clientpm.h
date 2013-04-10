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
//this class manages the client side of a connection

#ifndef CLIENTPM_H
#define CLIENTPM_H

#ifdef WIN32
#include <winsock2.h>
#endif
#include "packetmanager.h"
#include "messages.h"

class clientpm: public packetmanager
{
private:
	SDL_Thread* thread;
	bool running;
	Uint32 ip;
	Uint16 port;
	Uint8  data[MAXPACKET];	
	Uint16 sid;
	Uint32 sock;
#ifndef WIN32
	struct addrinfo *res;
#else
	struct addrinfo {
		sockaddr_in * ai_addr;
		int ai_addrlen;
		int ai_family;
		int ai_flags;
		int ai_protocol;
		int ai_socktype;
	} * res, realres;
	struct sockaddr_in sin;
#endif
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

