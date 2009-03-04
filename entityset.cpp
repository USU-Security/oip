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
#include "entityset.h"
#include "ips.h"
#include <stdio.h>

#define MAXENTITIES 100

void entityset::loadcolors()
{
	vector<string> clist;
	config.values("customcolor", clist);
	vector<string>::iterator i;
	for (i = clist.begin(); i != clist.end(); i++)
	{
		int pos = (*i).find(" ");
		string net = (*i).substr(0, pos);
		string color = (*i).substr(pos+1);
		pos = net.find("/");
		string ip = net.substr(0, pos);
		string mask;
		if (pos == net.npos)
			mask = "32";
		else
			mask = net.substr(pos+1);
		pos = color.find("x");
		if (pos != color.npos)
			color = color.substr(pos+1);
		unsigned int iip, imask, icolor;
		iip = iptolong(ip.c_str());
		int cidr = atoi(mask.c_str());
		imask = 0;
		unsigned int m = 0x80000000;
		while (cidr)
		{
			imask |= m;
			m >>= 1;
			cidr--;
		}
		imask = ((imask & 0xff000000) >> 24) | 
				((imask & 0x00ff0000) >> 8) | 
				((imask & 0x0000ff00) << 8) | 
				((imask & 0x000000ff) << 24);
		sscanf(color.c_str(), "%x", &icolor);
		colors.push_back(colornet(iip&imask, imask, icolor));
	}
}

entity& entityset::add(int s)
{
	static entity badentity; // default constructor sets the invalid flag
	entity& r = elist[s];
	if (!r.isvalid() && elist.size() < MAXENTITIES)
	{	
		entity& r2 = elist[s] = entity(s);
		//modify the fade rate based on the count of objects. 
		entity::faderateset(elist.size());
		//check to see if the new host has a color set
		vector<colornet>::iterator i;
		for (i = colors.begin(); i != colors.end(); i++)
		{
			if (((*i).mask & s) == (*i).ip)
			{
				r2.color = (*i).color;
				break;
			}
		}
		return r2;
	}
	else if (r.isvalid())
		r.touch();
	else
	{
		elist.erase(s);
		return badentity;
	}
	return r;
}

float ipdist(unsigned int s1, unsigned int s2)
{
	if ((s1 & 0xff000000) == (s2 & 0xff000000))
	{
		if ((s1 & 0xffff0000) == (s2 & 0xffff0000))
		{
			if ((s1 & 0xffffff00) == (s2 & 0xffffff00))
				return 1.75;
			else
				return 1.5;
		}
		return 1.25;
	}
	else
		return 1;
}


void entityset::process(double dt)
{
	//go through all of the entities, and apply hookes law to each of them
	map<int, entity>::iterator i = elist.begin();
	for(;i != elist.end(); i++)
	{
		float fx=0, fy=0;
		map<int, entity>::iterator j = elist.begin();
		//sum up the total force applied by all the others
		float rsq;
		for (;j != elist.end(); j++)
		{
			if ((*i).first != (*j).first)
			{
				float nk = k * ipdist((*i).second.label, (*j).second.label);
				//hookes law; linear attraction
				fx -= nk*(((*i).second.getX() - (*j).second.getX()));
				fy -= nk*(((*i).second.getY() - (*j).second.getY()));
				//coulomb repulsion
				rsq = 
					((*i).second.getX() - (*j).second.getX()) *
					((*i).second.getX() - (*j).second.getX()) +
					((*i).second.getY() - (*j).second.getY()) *
					((*i).second.getY() - (*j).second.getY());
				//some magic numbers. The "charge" of the particle goes away as it fades
				rsq /= k/(4); // * (1+(*j).second.getfadeval()));
				fx += ((*i).second.getX() - (*j).second.getX())/rsq;
				fy += ((*i).second.getY() - (*j).second.getY())/rsq;
			}
			//add a slight jitter so that they don't get stuck in the middle
			fx += (float)rand()/RAND_MAX * .001 - .0005;
			fy += (float)rand()/RAND_MAX * .001 - .0005;
			//rather than using hookes law, what if we just repulse from the sides?
			//they should distribute themselves neatly
			//They appear to just slap themselves on the sides
			/*
			//left side
			rsq = 
				((*i).second.getX()) *
				((*i).second.getX());
			rsq /= k/100;
			fx += ((*i).second.getX())/rsq;
			//right side
			rsq = 
				((*i).second.getX() - 1) *
				((*i).second.getX() - 1);
			rsq /= k/100;
			fx += ((*i).second.getX() - 1)/rsq;
			//top
			rsq = 
				((*i).second.getY()) *
				((*i).second.getY());
			rsq /= k/100;
			fy += ((*i).second.getY())/rsq;
			//bottom
			rsq = 
				((*i).second.getY() - 1) *
				((*i).second.getY() - 1);
			rsq /= k/100;
			fy += ((*i).second.getY() - 1)/rsq;
			// */
		}
		


		(*i).second.move(fx, fy, damp, dt);
	}
}

void entityset::draw(SDL_Surface*s)
{
	xscale = s->w;
	yscale = s->h;
	
	map<int, entity>::iterator j = elist.begin();
	//draw them, and do some housekeeping
	bool erasedsome = false;
	while (j != elist.end())
	{
		(*j).second.draw(s);
		if ((*j).second.deleteme())
		{
			elist.erase(j++);
			erasedsome = true;
		}
		else
			j++;
	}
	if (erasedsome)
		entity::faderateset(elist.size());
}

/*
 * find an entity that contains the location, given in screen coords
 */
entity* entityset::find(int x, int y)
{
	map<int, entity>::iterator j = elist.begin();
	for (;j != elist.end();j++)
	{
		//TODO getX and getY are in world coordinates, yet getW and getH are not
		if (
			(*j).second.getX() * xscale - (*j).second.getW()/2 < x &&
			(*j).second.getY() * yscale - (*j).second.getH()/2 < y &&
			(*j).second.getX() * xscale + (*j).second.getW()/2 > x &&
			(*j).second.getY() * yscale + (*j).second.getH()/2 > y
			)
			return &((*j).second);
	}
	return NULL;
}

