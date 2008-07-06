//this class will do reverse name lookups on ip addresses. 
//because these name lookups can be slow, and may even time out, 
//maybe we should do these asynchronously

#ifndef NAMECACHE_H
#define NAMECACHE_H

#include <map>
#include <string>
using std::map;
using std::string;

class namecache
{
private:
	map<unsigned int , string> names;

public:
	const string & operator[](unsigned int a);
	void remove(unsigned int a) {names.erase(a);}
};

#endif //NAMECACHE_H

