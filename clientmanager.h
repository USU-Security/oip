
#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "networkpm.h"
#include "messages.h"
#include <vector>
#include <map>
#include <string>
#include <SDL/SDL_thread.h>

using namespace std;

typedef void (*connectioncallback)(networkpm & pm, const map<string, string>&);

class client
{
public:
	Uint32 ip;
	Uint32 port;
	Uint32 lastsent;
	networkpm dataset;
	Uint16 sid;
	client(Uint32 s, Uint16 p, int l, Uint16 id):ip(s),port(p),lastsent(l),sid(id) {}
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

