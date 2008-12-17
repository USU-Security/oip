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
#include "myconfig.h"
#include <iostream>
#include <fstream>
#include "ips.h"
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
				while (*val && (*val == ' ' || *val == '='))
					val++;
				//set the value. 
				opts.insert(pair<string, string>(buffer, val));
				//opts[buffer] = val;
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


const string & configfile::value(const char* k)
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

unsigned int configfile::ipvalue(const char* k)
{
	return iptolong(value(k).c_str());
}

void configfile::values(const char* key, vector<string> & ret)
{
	pair<multimap<string, string>::iterator, multimap<string, string>::iterator> range = 
		opts.equal_range(key);
	multimap<string, string>::iterator i;
	for (i = range.first; i != range.second; i++)
		ret.push_back((*i).second);
}

