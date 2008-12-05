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
#include <iostream>
#include <iomanip>
#include "clientmanager.h"
#include "clientpm.h"
#include <map>
#include <string>
#include <sstream>
using namespace std;

string hexit(Uint8* b, int size)
{
	stringstream oss;
	int i;
	for (i = 0; i < size; i++)
		oss << hex << setw(2) << setfill('0') << (int)b[i] ;
	return oss.str();
}

int errcount=0;

#define ASSERTEQUAL(a, b) {if ((a) != (b)) {errcount++;cerr << (#a) << " != " << (#b) << " on line " << __LINE__ << "\n";}}
#define ASSERTNOTEQUAL(a, b) {if ((a) == (b)) {errcount++;cerr << (#a) << " != " << (#b) << " on line " << __LINE__ << "\n";}}
#define ASSERTTRUE(a) {if ((a)) {} else {errcount++;cerr << (#a) << " is not true on line " << __LINE__ << "\n";}}
#define SUMMARY {if (errcount) {cout << "There were " << errcount << " failed tests\n";} else { cout << "All the tests passed\n"; } }


void newconnection(networkpm& pm, const map<string, string>& opts)
{
	ASSERTNOTEQUAL(opts.find("one"), opts.end());
	ASSERTEQUAL((*opts.find("one")).second, "1");
	ASSERTNOTEQUAL(opts.find("two"), opts.end());
	ASSERTEQUAL((*opts.find("two")).second, "2");
	ASSERTNOTEQUAL(opts.find("three"), opts.end());
	ASSERTEQUAL((*opts.find("three")).second, "3");
	ASSERTNOTEQUAL(opts.find("four"), opts.end());
	ASSERTEQUAL((*opts.find("four")).second, "4");
	ASSERTNOTEQUAL(opts.find("five"), opts.end());
	ASSERTEQUAL((*opts.find("five")).second, "5");
	ASSERTNOTEQUAL(opts.find("six"), opts.end());
	ASSERTEQUAL((*opts.find("six")).second, "6");
	ASSERTNOTEQUAL(opts.find("seven"), opts.end());
	ASSERTEQUAL((*opts.find("seven")).second, "7");
	ASSERTNOTEQUAL(opts.find("eight"), opts.end());
	ASSERTEQUAL((*opts.find("eight")).second, "8");
	ASSERTNOTEQUAL(opts.find("nine"), opts.end());
	ASSERTEQUAL((*opts.find("nine")).second, "9");
	ASSERTNOTEQUAL(opts.find("ten"), opts.end());
	ASSERTEQUAL((*opts.find("ten")).second, "10");
	int i, j;
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 100; j++)
		{
			ASSERTEQUAL(pm.addpacket(i, i+10, i+100, 10), true);
		}
	}
	pm.producerdead();
}


int testnetwork()
{
	//two objects:
	//the clientmanager will listen for connections, and notify us via callback
	//when we get them. 
	clientmanager* cm = new clientmanager(newconnection);
	
	//the clientpm is actually an instance of packetmanager that connects to
	//the clientmanager to get its data. 
	map<string, string> m;
	m["one"] = "1";
	m["two"] = "2";
	m["three"] = "3";
	m["four"] = "4";
	m["five"] = "5";
	m["six"] = "6";
	m["seven"] = "7";
	m["eight"] = "8";
	m["nine"] = "9";
	m["ten"] = "10";
	//test the initialization
	clientpm* cpm = new clientpm("localhost", m);
	ASSERTEQUAL(cpm->alive(), true);
	do
	{
		SDL_Delay(1); //go to sleep
	}
	while(!cpm->size());
	
	//test the data transfer
	ASSERTEQUAL(cpm->size(), 10);
	pmdict::const_iterator i;
	for (i = cpm->begin(); i != cpm->end(); ++i)
	{
		int s = (*i).first.src;
		ASSERTEQUAL((*i).first.dst, s+10);
		ASSERTEQUAL((*i).first.color, s+100);
		ASSERTEQUAL((*i).second, 1000);
	}

	//see if it can die gracefully
	//(should send the killstream message, and then the cp wants to die)
	delete cpm;

	//give it a chance to die, but then kill it after 10 seconds
	int now = SDL_GetTicks();
	do
	{
		SDL_Delay(100);
	}
	while(cm->size() == 1 && now + 10000 > SDL_GetTicks());

	ASSERTEQUAL(cm->size(), 0);
	delete cm;	
}

int testpackets()
{
	Uint8 buffer[MAXPACKET*2] = {0};
	//test out the setup packet
	map<string, string> m;
	m["one"] = "1";
	m["2"] = "two";
	string badstring("badstring");
	string::iterator i = badstring.begin();
	i++;
	badstring.insert(i, 2000, 'a');
	m["bad"] = badstring;
	setuppacket sp(buffer);
	sp.setsequence(0xffffffff);
	sp.setid(0xbeaf);
	sp.setopts(m);
	ASSERTEQUAL(hexit(buffer, sp.getsize()), "ffffffff3030303030303030303030300000afbe0200010032030074776f03006f6e65010031");
	setuppacket sp2(buffer, sp.getsize());
	ASSERTEQUAL(sp2.getid(), 0xbeaf);
	ASSERTEQUAL(sp2.gettype(), SETUP);
	ASSERTEQUAL(sp2.getsequence(), 0xffffffff);

	datapacket dp(buffer);
	dp.setsequence(0xffffffff);
	dp.setid(0xdead);
	dp.adddata(0,1,2,3);
	dp.adddata(4,5,6,7);
	ASSERTEQUAL(hexit(buffer, dp.getsize()), "ffffffff3030303030303030303030300003adde02000000000001000000020000000300000004000000050000000600000007000000");
	datapacket dp2(buffer, dp.getsize());
	ASSERTEQUAL(dp2.count(), 2);


}


int main()
{
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
	testpackets();
	testnetwork();
	SDL_Quit();

	SUMMARY;
	return 0;
}
