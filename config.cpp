#include "config.h"
#include <iostream>
#include <fstream>
using namespace std;

configfile config;

#define BUFFSIZE 128

configfile::configfile(const char* file)
{
	int lineno  = 0;
	char buffer[BUFFSIZE];
	ifstream fin(file);
	do
	{
		lineno++;
		//grab  a line from the file
		fin.getline(buffer, BUFFSIZE);
		char* val = buffer;
		//discard it if it begins with a #
		if (*val != '#' && *val)
		{
			//loop until you find whitespace
			while (*val && *val != ' ')
				val++;
			if (*val)
			{
				*val = 0;
				val++;
				//loop while there is whitespace
				while (*val && *val == ' ')
					val++;
				//set the value. 
				opts[buffer] = val;
				//cout << "line " << lineno << ": '" <<  buffer << " = " << val << "'\n";
			}
			else
			{
				cerr << "Line " << lineno << ": " << buffer << "does not have a value.\n";
			}
		}
	}
	while (!fin.fail());
}


const string & configfile::operator[](const char* k)
{
	map<string,string>::iterator i;
	i = opts.find(k);
	static string empty("");
	if (i == opts.end())
	{
		cerr << "Value " << k << " is missing from the config file\n";
		return empty;
	}
	return (*i).second;
}

	
