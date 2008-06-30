
#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "networkpm.h"
#include "messages.h"
#include <vector>
#include <map>
#include <string>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_net.h>

using namespace std;


typedef void (*connectioncallback)(networkpm & pm, const map<string, string>&);

class client
{
public:
	IPaddress ip;
	Uint32 lastsent;
	networkpm dataset;
	Uint16 sid;
	client(IPaddress &s, int l, Uint16 id):ip(s),lastsent(l),sid(id) {}
};

class clientmanager
{
private:
	vector<client*> datasets;
	static int serverthread(void*);
	connectioncallback ccallback;
	unsigned short listenport;
	bool running;
	SDL_Thread *thread;
public:
	clientmanager(connectioncallback cc, unsigned short port= 12751);
	~clientmanager(); 

	int size() { return datasets.size(); }
};

#endif //CLIENTMANAGER_H

