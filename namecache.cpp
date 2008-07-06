#include "namecache.h"
#include <sys/socket.h>
#include <netdb.h>
#include "ips.h"
#include <iostream>
using std::cout;

const string & namecache::operator[](unsigned int a)
{
	map<unsigned int, string>::iterator i;
	i = names.find(a);
	if (i != names.end())
		return (*i).second;
	
	struct sockaddr_in addr;
	addr.sin_family = PF_INET;
	addr.sin_addr.s_addr = a;
	char host[256];
	char serv[256];
	if (getnameinfo((struct sockaddr*)&addr, sizeof(addr), host, 256, serv, 256, NI_NOFQDN))
	{
		cout << "Looked up '" << host << "'\n";
		return names[a] = longtoip(host, 256, a);
	}
	else
		return names[a] = host;
}

