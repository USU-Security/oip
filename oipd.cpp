/*
 * This file tests the entity manager
 */

#include <iostream>
#include <set>
using namespace std;

//#include <sys/time.h>
#include <SDL/SDL.h>
#include <stdlib.h>
#include <pcap.h>
#include "structs.h"
#include <arpa/inet.h>
#include "clientmanager.h"
#include <signal.h>

#define SNAPLEN 80
	
bool globalrun=true;
const char* sniff_dev = 0;
//can use insert(), erase()
set<SDL_Thread*> clients;

struct sniffargs
{
	map<string, string> opts;
	networkpm & pm;
	sniffargs(networkpm&p, map<string, string> o):pm(p),opts(o) {}
};

int sniff(void* a)
{
	sniffargs* self = (sniffargs*)a;
	struct pcap_pkthdr* header;
	const u_char* packet;
	pcap_t * handle = NULL;
	const struct sniff_ethernet *ethernet;
	const struct sniff_ip *ip;
	int result;
	bool clientrun = true;
	const char* dev = sniff_dev;
	char errbuf[PCAP_ERRBUF_SIZE];
	if (!dev)
		dev = pcap_lookupdev(errbuf);
	if (clientrun)
	{
		//open up the device
		handle = pcap_open_live(dev, SNAPLEN, 1, 100, errbuf);
		if (!handle)
		{
			cerr << "Unable to open " << dev << ": " << errbuf << endl;
			clientrun = false;
		}
	}
	if (!dev)
	{
		cerr << "Unable to open default device: " << errbuf << endl;
		cerr << "Attempting to use eth0\n";
		dev = "eth0";
	}
	if (self->opts["filter"] != "")
	{
		struct bpf_program fp;
		if (pcap_compile(handle, &fp, self->opts["filter"].c_str(), 1, 0))
		{
			clientrun = false;
			cerr << "Couldn't parse filter: '" << self->opts["filter"] << "': " << pcap_geterr(handle) << "\n";
		}
		else if (pcap_setfilter(handle, &fp))
		{
			clientrun = false;
			cerr << "Couldn't set filter: '" << self->opts["filter"] << "': " << pcap_geterr(handle) << "\n";
		}
	}
	
	int retry = 5;
	while(globalrun && clientrun)
	{
		result = pcap_next_ex(handle, &header, &packet);
		if (1 == result)
		{
			retry = 5;
			ethernet = (struct sniff_ethernet*)packet;
			if (ntohs(ethernet->ether_type) == T_IP)
				ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
			if (ntohs(ethernet->ether_type) == T_VLAN)
			{
				struct sniff_ethernet_8021q*  vethernet = (struct sniff_ethernet_8021q*)packet;
				//cout << "read a packet from vlan " << VLANID(ntohs(vethernet->vlan_id)) << "\n";
				if (ntohs(vethernet->ether_type) == T_IP)
				{
				//	cout << "\tand it was an ip packet\n";	
					ip = (struct sniff_ip*)(packet + SIZE_8021Q);
				}
			}
			if (ip)
			{
				if (ip->ip_p == T_TCP) //could use header.len/1400.0 or something
					clientrun = self->pm.addpacket(ip->ip_src, ip->ip_dst, 0xff00ff00, header->len);
				else if (ip->ip_p == T_UDP)
					clientrun = self->pm.addpacket(ip->ip_src, ip->ip_dst, 0xffff0000, header->len);
				else
					clientrun = self->pm.addpacket(ip->ip_src, ip->ip_dst, 0xff0000ff, header->len);
				ip = 0;
			}
		}
		else if (result == -1)
		{
			cerr << "Unable to read packet: " << pcap_geterr(handle) << "\n";
			retry--;
			if (!retry)
				clientrun = false;
		}
	}
	self->pm.producerdead(); //notify the world that we are quitting
	if (handle)
		pcap_close(handle);
	delete self;
	return 0;
}

void newclient(networkpm& pm, const map<string, string>& opts)
{
	SDL_Thread* tid = SDL_CreateThread(sniff, new sniffargs(pm, opts));
	if (tid)
		clients.insert(tid);
}

void cleanup()
{
	globalrun = false;
	set<SDL_Thread*>::iterator i;
	for (i = clients.begin(); i != clients.end(); ++i)
		SDL_WaitThread(*i, NULL);
	SDL_Quit();
}
bool setup()
{
	if (SDL_Init(SDL_INIT_TIMER))
		return false;
	atexit(cleanup);
	return true;
}

void sigcatch(int s)
{
	cout << "Quitting...\n";
	exit(0);
}


int main(int argc, char** argv)
{
	if (argc > 1)
		sniff_dev = argv[1];
			
	signal(SIGINT, sigcatch);
	//get a device to read from
	if (setup())
	{
		clientmanager server(newclient);
		while(globalrun)
		{
			SDL_Delay(1000);
		}

	}
	return 0;
}

