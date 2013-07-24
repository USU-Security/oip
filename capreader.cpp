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
#include "capreader.h"

#include <arpa/inet.h>

#define TVT(t) ((t).tv_sec + (t).tv_usec / 1000000.0)


capreader::capreader(const char* file)
{
	errbuf[0] = 0;
	handle = pcap_open_offline(file, errbuf);
	result = 0;
	if (handle) //get the first packet
	{
		result = pcap_next_ex(handle, &header, &packet);
		start = TVT(header->ts);
	}
	if (result != 1)
		throw "Unable to open the file";
}

capreader::~capreader()
{
	if (handle)
		pcap_close(handle);
}

//bool capreader::dumpdata(packetsink& ps)
bool capreader::dumpdata(packetsink& ps, int speed)
{
	//we do these in reverse order so that peek will work
	
	// step 2. actually dump it
	//bool ret = packetmanager::dumpdata(ps);	
	bool ret = packetmanager::dumpdata(ps, speed);	
	
	//step 1. load the data
	const struct sniff_ethernet *ethernet;
	const struct sniff_ip*ip = NULL;
    // Normal
	//current += (SDL_GetTicks()/1000.0 - ti) * rate;
	//ti = SDL_GetTicks()/1000.0;
    
    // 100x?
	current += (SDL_GetTicks()/speed - ti) * rate;
	ti = SDL_GetTicks()/speed;

	double stamp = TVT(header->ts);
	while (result == 1 && current > (stamp - start))
	{	
		ethernet = (struct sniff_ethernet*)packet;
		if (ntohs(ethernet->ether_type) == T_IP)
			ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
		if (ntohs(ethernet->ether_type) == T_VLAN)
		{
			struct sniff_ethernet_8021q*  vethernet = (struct sniff_ethernet_8021q*)packet;
			//cout << "read a packet from vlan " << VLANID(ntohs(vethernet->vlan_id)) << "\n";
			if (ntohs(vethernet->ether_type) == T_IP)
			{
			//  cout << "\tand it was an ip packet\n";  
				ip = (struct sniff_ip*)(packet + SIZE_8021Q);
			}
		}
		if (ip)
		{
			if (ip->ip_p == T_TCP) //could use header.len/1400.0 or something
				addpacket(ip->ip_src, ip->ip_dst, 0xff00ff00, header->len);
			else if (ip->ip_p == T_UDP)
				addpacket(ip->ip_src, ip->ip_dst, 0xffff0000, header->len);
			else
				addpacket(ip->ip_src, ip->ip_dst, 0xff0000ff, header->len);

			ip = NULL;
		}
		result = pcap_next_ex(handle, &header, &packet);
		stamp = TVT(header->ts);
	}
	return ret;
}
