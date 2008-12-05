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
//this class will do reverse name lookups on ip addresses. 
//because these name lookups can be slow, and may even time out, 
//maybe we should do these asynchronously

#ifndef NAMECACHE_H
#define NAMECACHE_H

#include <string>
using std::string;

#ifndef	WIN32
#include <ext/hash_map>
typedef __gnu_cxx::hash_map<unsigned int, string> namehash;
#else
#include <map> //vc++ 6 doesnt have hash_map
typedef std::map<unsigned int, string> namehash;
#endif
#include <string>
#include <deque>
using std::string;
using std::deque;
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>



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

extern namecache names;

#endif //NAMECACHE_H

