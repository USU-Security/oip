/*
 * handles configuration files
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>
using std::string;
using std::map;

class configfile
{
private:
	map<string, string> opts;
public:
	configfile(const char* file="oip.conf");
	const string& value(const char* key);

};

extern configfile config;

#endif //CONFIG_H
