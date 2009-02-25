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

#ifndef NETWORKPM_H
#define NETWORKPM_H

#include "packetmanager.h"

//this class reimplements the packetmanager so that we can dump our particles
//to a datapacket instead
class networkpm: public packetmanager
{
private:
public:
	bool consumer,producer;
	networkpm():packetmanager(),consumer(true),producer(true)  {}
	bool addpacket(Uint32 s, Uint32 d, Uint32 c, Uint32 size) {if (consumer) packetmanager::addpacket(s,d,c,size); return consumer;}

	void consumerdead() {consumer=false;}
	void producerdead() {producer=false;}
	bool killme() {return not (producer || consumer); }
};

#endif //NETWORKPM_H
