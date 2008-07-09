//this class will do reverse name lookups on ip addresses. 
//because these name lookups can be slow, and may even time out, 
//maybe we should do these asynchronously

#ifndef NAMECACHE_H
#define NAMECACHE_H

#include <ext/hash_map>
#include <string>
#include <deque>
using std::string;
using std::deque;
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

typedef __gnu_cxx::hash_map<unsigned int, string> namehash;

class namecache
{
private:
	namehash names;
	static int cthread(void*);
	void dolookup(unsigned int a);
	bool lookupwait;
	SDL_mutex* queuemutex;
	SDL_cond* queuedata;
	deque<unsigned int > queue;
	SDL_Thread* tid;
	bool running;
	bool dead;
public:
	namecache();
	~namecache();
	const string & operator[](unsigned int a);
	void remove(unsigned int a) {names.erase(a);}
};

#endif //NAMECACHE_H

