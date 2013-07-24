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
#ifndef CAPREADER_H
#define CAPREADER_H

#include "intervalreader.h"
#include <pcap.h>
#include "structs.h"

class capreader : public intervalreader
{
private:
	pcap_t * handle;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct pcap_pkthdr* header;
	const u_char* packet;
	int result;
public:
	capreader(const char* file);
	~capreader();

	//bool dumpdata(packetsink&);
	bool dumpdata(packetsink&, int speed );
	const char* error() { return errbuf;}

	bool eof() { return result == -2;}
};

#endif//CAPREADER_H
