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
#include "messages.h"
#include <iostream>

using std::cerr;

packet::packet(Uint8* buffer, int s)
:data(buffer),size(s) 
{
	if (!s) 
	{
		setreserved(); 
		setversion();
	} 
	else if (getversion() != VERSION)
		cerr << "Version mismatch. Recieved packet of version " << (int)getversion() << " but expected version " << VERSION << "\n";
	psize=0;	
}

int packet::paddedsize(int kl)
{ 
	int i;
	if (!psize)
	{
		int s = getsize();
		psize = ((s - 1)/kl + 1) * 16; 
		for (i = s; i < psize; i++) 
			data[i]=rand()%256; 
	}
	return psize;
}



setuppacket::setuppacket(Uint8*buffer, int s)
:packet(buffer, s),setupsize(0)
{
	settype(SETUP);
}

void setuppacket::setopts(const map<string, string> &m)
{
	int pos = packet::getsize() + 2;
	//put16inc(pos, m.size());
	int optcount = 0;
	map<string, string>::const_iterator i = m.begin();
	for (;i != m.end(); i++)
	{
		//check to see if the data they want to add is too long
		if (pos + (*i).first.length() + (*i).second.length() + 4 >= MAXPACKET)
			continue; //skip it. 
		putstringinc(pos, (*i).first);
		putstringinc(pos, (*i).second);
		optcount++;
	}
	put16(packet::getsize(), optcount);
		
	setupsize = pos - packet::getsize();
}

void setuppacket::getopts(map <string, string> &m)
{
	int pos = packet::getsize();
	int count = get16inc(pos);
	int i;
	for (i = 0; i < count; i++)
	{
		string s = getstringinc(pos);
		m[s] = getstringinc(pos);
	}
	setupsize = pos - packet::getsize();
}

datapacket::datapacket(Uint8*buffer, int s)
:packet(buffer, s)
{
	if (!s) 
	{
		settype(DUMPPACKETS);
		size = packet::getsize();
		put16(size, 0);		
	}
}

bool datapacket::addpacket(Uint32 src, Uint32 dst, Uint32 color, Uint32 count)
{
	int pos = getsize();
	if (MAXPACKET < pos + 16)
		return false; //unable to add more data
	put32inc(pos, src);
	put32inc(pos, dst);
	put32inc(pos, color);
	put32inc(pos, count);
	//char b1[16], b2[16];
	//cout << longtoip(b1, 16, src) << " --> " << longtoip(b2, 16, dst) << "\n";
	size = pos;
	inccount();
	return true;
}

void datapacket::dumpdata(packetmanager &ps)
{
	char b1[16], b2[16];
	int pos = packet::getsize()+2;
	int i;
	int c = count();
	for (i = 0; i < c; i++)
	{
		Uint32 s,d,c,count;
		s = get32inc(pos);
		d = get32inc(pos);
		c = get32inc(pos);
		//cout << longtoip(b1, 16, s) << " --> " << longtoip(b2, 16, d) << "\n";
		count = get32inc(pos);
		ps.addpacket(s,d,c,count);
	}
}

