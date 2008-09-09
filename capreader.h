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

	bool dumpdata(packetsink&);
	const char* error() { return errbuf;}

	bool eof() { return result == -2;}
};

#endif//CAPREADER_H
