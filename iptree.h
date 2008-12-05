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
#ifndef IPTREE_H
#define IPTREE_H

//
// A binary tree built to store ip addresses. The addresses are stored at the
// leaves, and each branch represents a bit in the address. 
// 
// this gives us the following properties:
// 	  the data is easily grouped into networks if the number of children for
// 	  a particular node grows too large
//
// 	  the network distance can easily be found, and used as a metric to see
// 	  how far apart they should drift. 
//
// 	  The tree depth is fixed
//
// This tree should basically be a drop in for the map used by the entityset
//

// This particular implementation defines a bucket for the first n bits of the
// tree, with the assumption that only cidr masks > n are interesting. This is
// to facilitate a speed/memory tradeoff. 

// The number of bits in the bucket
#define BBITS 16
// how many real bits left over
#define RBITS (32 - BBITS)


#include "entity.h"

struct ipnode
{
	ipnode* left;
	ipnode* right;
	unsigned int count;
	entity *e;
	void initc() {left = right = NULL; count = 0;e=NULL;}
	void inite() {initc();}
};

class iptree
{
private:
	ipnode* root[1 << BBITS];
	

	void inc(unsigned int i);
	void dec(unsigned int i);
	int maxdisp;

public:	
	class iterator
	{
	private:
		ipnode* tmp;
	    ipnode* is[RBITS];
		ipnode** root;
		int ispos;
		int ds[RBITS];
		int dspos;
    	unsigned int i;
		bool end;
		bool full;
	
	public:
		iterator(bool isend=false, bool f = false):end(isend),full(f) {}
		iterator(ipnode** r, bool f=false);
		entity & operator * () {return *tmp->e;}
		entity * operator->() { return tmp->e;}
		iterator & operator++();
		bool operator==(const iterator& o) 
		{ return (o.end == true && end == true) || o.tmp == tmp;}
		bool operator!=(const iterator& o)
		{ return !(*this == o);}
	};
	
	iptree(int m=32):maxdisp(32) {unsigned int i; for (i = 0; i < (1<<BBITS); i++) root[i] = NULL;}

	entity& add(unsigned int s);
	void del(unsigned int s) {dec(s);}
	entity& find(int x, int y);
	entity& operator[](unsigned int s) {return add(s);}

	iterator  begin() {return iterator(root);}
	iterator  beginfull() {return iterator(root, true);}
	iterator  end() {return iterator(true);}

	int getcount(unsigned int i, int c);
};



#endif//IPTREE_H
