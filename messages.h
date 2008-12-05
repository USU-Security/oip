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
#ifndef MESSAGES_H
#define MESSAGES_H

#include <SDL/SDL.h>

#include "packetmanager.h"
#include "packetsink.h"
#include <map>
#include <string>
using namespace std;

//the maximum length of a packet
//has to be a multiple of 16 (128 bit keylength for aes)
#define MAXPACKET 1440
#define MINRATE 41
#define VERSION 0

enum packet_type {SETUP, SETUPACK, SENDDATA, DUMPPACKETS, ENDDATA};

//the messages that we will send and recieve
//the buffer in here comes from somewhere else. 
class packet
{
protected:
	Uint8* data;
	int size;
	int psize;
	void put8(int pos, Uint8 d) {data[pos] = d;}
	void put8inc(int &pos, Uint8 d) {put8(pos, d); pos++;}
	Uint8 get8(int pos) {return pos < size ? data[pos] : 0; }
	Uint8 get8inc(int& pos) {return get8(pos++);}
	void put16(int pos, Uint16 d) {*(Uint16*)(data + pos) = d;}
	void put16inc(int &pos, Uint16 d) {put16(pos, d); pos+=2;}
	Uint16 get16(int pos) {return pos + 1 < size ? *(Uint16*)(data + pos): 0;}
	Uint16 get16inc(int &pos) {Uint16 ret = get16(pos); pos+=2; return ret;}
	void put32(int pos, Uint32 d) {*(Uint32*)(data + pos) = d;}
	void put32inc(int &pos, Uint32 d) {put32(pos, d); pos+=4;}
	Uint32 get32(int pos) {return pos + 3 < size ? *(Uint32*)(data + pos): 0;}
	Uint32 get32inc(int& pos) {Uint32 ret = get32(pos); pos+=4; return ret;}
	void putstring(int pos, const string& s) { put16(pos, s.length()); memcpy(data + pos + 2, s.c_str(), s.length()); }
	void putstringinc(int &pos, const string& s) { putstring(pos, s); pos += (2 + s.length());}
	string getstring(int pos) {int l = get16(pos); return pos + l + 1 < size ?string((const char*)(data+pos+2), l):string("");}
	string getstringinc(int & pos) {string ret = getstring(pos); pos += (2 + ret.length()); return ret; }
	void settype(packet_type t) {put8(17, t);}
public:
	packet(Uint8* buffer, int s=0);
	Uint32 getsequence() { return get32(0); }
	void setsequence(Uint32 s) { put32(0, s);}
	void setreserved() {memset(data + 4, '0', 12);} //don't want to send logical garbage
	Uint8 getversion() { return get8(16);}
	void setversion() {put8(16, 0);}
	packet_type gettype() { return (packet_type)get8(17); }
	Uint16 getid(){ return get16(18);}
	void setid(Uint16 id) {put16(18, id);}

	virtual int getsize() { return 20;}
	int paddedsize(int kl=16); 
	Uint8* getdata() { return data;}

};

class setuppacket : public packet
{
protected:
	int setupsize;
public:
	setuppacket(Uint8*buffer, int s=0);
	void getopts(map<string, string> & m);
	void setopts(const map<string, string> & m);

	int getsize() { return packet::getsize() + setupsize; }
};

class datapacket : public packet, public packetsink
{
protected:
	void inccount() {put16(packet::getsize(), count()+1);}
public:
	datapacket(Uint8*buffer, int s=0);
	virtual bool addpacket(Uint32 src, Uint32 dst, Uint32 color, Uint32 count);
	void dumpdata(packetmanager& pm);
	int count() { return get16(packet::getsize());}
	int getsize() { return packet::getsize() + count() * 16 + 2; }
#ifndef WIN32 //yet another workaround because of a msvc++ bug
	static const int MAXDATA = (MAXPACKET - 20)/16;
#else
	enum {MAXDATA = (MAXPACKET - 20)/16};
#endif
};

//no extra data, just the message type 
class senddata : public packet
{
public: 
	senddata(Uint8*buffer, int s=0):packet(buffer, s) {if (!s) settype(SENDDATA);}
};

//no extra data, just the messge type
class enddata : public packet
{
public:
	enddata(Uint8*buffer, int s=0):packet(buffer, s) {if (!s) settype(ENDDATA);}
};


#endif //MESSAGES_H

