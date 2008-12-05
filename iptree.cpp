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
#include "iptree.h"

void iptree::inc(unsigned int s)
{
	unsigned int wbucket = s >> (RBITS);
	ipnode* tmp = root[wbucket];
	int i;
	for (i = RBITS - 1; i > 0; i--)
	{
		tmp->count++;
		if (s & (1 << i))
			tmp = tmp->right;
		else
			tmp = tmp->left;
	}
}
void iptree::dec(unsigned int s)
{
	unsigned int wbucket = s >> (RBITS);
	ipnode* tmp = root[wbucket];
	ipnode** t1, *t2;
	t1 = & root[wbucket];
	if (tmp->count == 1)
		root[wbucket] = NULL;
		
	int i;
	for (i = BBITS; i < BBITS + RBITS - 1 && tmp; i++)
	{
		tmp->count--;
		t2 = tmp;
		if (s & (1 << i))
		{
			tmp = tmp->right;
			if (tmp && tmp->count == 1)
				t2->right = NULL;
		}
		else
		{
			tmp = tmp->left;
			if (tmp && tmp->count == 1)
				t2->left = NULL;
		}
		if (t2->count == 0)
			delete t2;
	}
	if (tmp)
	{
		if (s & (1 << i))
		{
			delete tmp->right;
			tmp->right = NULL;
		}
		else
		{
			delete tmp->left;
			tmp->left = NULL;
		}
	}

}

entity& iptree::add(unsigned int s)
{
	unsigned int wbucket = s >> (RBITS);

	if (root[wbucket] == NULL)
	{
		root[wbucket] = new ipnode;
		root[wbucket]->initc();
	}
	ipnode* tmp = root[wbucket];
	int i;
	for (i = RBITS-1; i > 0; i--)
	{
		// Are we getting too many children in this branch?
		if (tmp->right && tmp->left && tmp->count > maxdisp)
		{
			//save an entity here
			if (tmp->e)
			{
				tmp->e->label = ntohl(s);
				tmp->e->count = tmp->count;
				tmp->e->touch();
			}
			else
				tmp->e = new entity(ntohl(s), tmp->count, i);
		}
		else if (tmp->e) //is there an entity here?
		{	//then it should be removed. 
			delete tmp->e;
			tmp->e = NULL;
		}
		if (s & (1 << i))
		{
			if (tmp->right == NULL)
			{
				tmp->right = new ipnode;
				tmp->right->initc();
			}
			tmp = tmp->right;
		}
		else
		{
			if (tmp->left == NULL)
			{
				tmp->left = new ipnode;
				tmp->left->initc();
			}
			tmp = tmp->left;
		}
	}
	if (s & 1)
	{
		if (tmp->right == NULL)
		{
			tmp->right = new ipnode;
			tmp->right->inite();
			tmp->right->e = new entity(ntohl(s));
			inc(s);
		}
		return *tmp->right->e;
	}
	else
	{
		if (tmp->left == NULL)
		{
			tmp->left = new ipnode;
			tmp->left->inite();
			tmp->left->e = new entity(ntohl(s));
			inc(s);
		}
		return *tmp->left->e;
	}
}


iptree::iterator::iterator(ipnode** r, bool f)
{
	full = f;
	end = false;
	root = r;
	ispos=0;
	dspos=0;
	for (i = 0; i < (1 << BBITS); i++)
	{
		if (root[i])
		{
			ispos = 1;
			is[0] = root[i];
			dspos = 1;
			ds[0] = BBITS;
			while (ispos)
			{
				tmp = is[--ispos];
				int depth = ds[--dspos];
				
				if (depth == BBITS+RBITS)
					return;
				else if (!full && tmp->e) //if its an abbreviated parse, 
					return; //stop here if there is something
				else 
				{
					if (tmp->right)
					{
						is[ispos++] = tmp->right;
						ds[dspos++] = depth + 1;
					}
					if (tmp->left)
					{
						is[ispos++] = tmp->left;
						ds[dspos++] = depth + 1;
					}
				}
			}
		}
	}
	end = true;
}



iptree::iterator& iptree::iterator::operator++()
{
	for(;;)
	{
		if (!ispos) //if the stack is empty, move to next bucket
		{
			i++;
			while (i < (1 << BBITS) && root[i] == NULL)
				i++;
			if (i >= (1 << BBITS)) //no more buckets
			{
				end = true;
				return *this;
			}
			is[ispos++] = root[i];
			ds[dspos++] = BBITS;
		}
		while (ispos)
		{
			tmp = is[--ispos];
			int depth = ds[--dspos];
			if (depth == 32)
				return *this;
			else if (!full && tmp->e)
				return *this; //stop here if its an abbreviated parse
			else
			{
				if (tmp->right)
				{
					is[ispos++] = tmp->right;
					ds[dspos++] = depth + 1;
				}
				if (tmp->left)
				{
					is[ispos++] = tmp->left;
					ds[dspos++] = depth + 1;
				}
			}
		}
	}
}

entity& iptree::find(int x, int y)
{
	unsigned int i = 0;
	unsigned int curbit = 31;

}

int iptree::getcount(unsigned int s, int c)
{
	unsigned int wbucket = s >> (RBITS);
	ipnode* tmp = root[wbucket];
	int i;
	for (i = RBITS - 1; i > c; i--)
	{
		tmp->count++;
		if (s & (1 << i))
			tmp = tmp->right;
		else
			tmp = tmp->left;
	}
	return tmp->count;	
}
