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

#include "entity.h"
#include <stdlib.h>
#ifndef WIN32
#include <arpa/inet.h>
#else
#include <winsock.h>
#endif 
#include "text.h"
#include "namecache.h"
#include "ips.h"
#define MAXFADERATE 1
#define MINFADERATE .0002
float FADERATE = .25;
#define UNFADERATE .031250
#define MAXENTITY 100
const float MINMOVE = .0000001;

#ifndef WIN32
#include <sys/time.h>
inline double now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec/1000000.0;
}
#else
inline double now()
{
	return SDL_GetTicks()/1000.0;
}
#endif
//set the fade rate based on the number of entities
void entity::faderateset(int count)
{
	FADERATE = MINFADERATE + (count * (MAXFADERATE - MINFADERATE) / MAXENTITY);
}
	

void entity::init(unsigned int l)
{
	brightness = 1;
	//generate a random position
	x = (float)rand()/RAND_MAX;
	y = (float)rand()/RAND_MAX;
	label = l;
	valid = true;
	fade = false;
	fadeval = 15;
	lastupdate = now();
	ndx = 0;
	ndy = 0;
	moving = true;
	resolve = true;
	faderate = 1;
	color = 0xffffffff;
}

entity::entity(unsigned int l)
{
	init(l);
}

void entity::move(float fx, float fy, float damp, double dt)
{
//	if (dx < MINMOVE && dy < MINMOVE)
//		stable = true;
//	else
	if (moving)
	{
		ndx = (ndx + fx * dt) * damp;
		ndy = (ndy + fy * dt) * damp;
	}
	else
	{
		ndx = 0;
		ndy = 0;
	}
	if (fade && fadeval < 15 && lastupdate + (FADERATE * faderate) < now() && moving)
	{
		fadeval++;
		lastupdate = now();
	}
	else if (!fade && fadeval >= 0 && lastupdate + UNFADERATE < now() && moving)
	{
		if (fadeval)
			fadeval--;
		else //let them start fading if they are reached full brightness
			fade = true;
		lastupdate = now();
	}
	
}

bool entity::draw(SDL_Surface* s)
{
	x += ndx;
	y += ndy;
	if (x < getW()/s->w/2) 
		x = getW()/s->w/2;
	else if (x > 1-getW()/s->w/2)
		x = 1-getW()/s->w/2;
	if (y < getH()/s->h/2)
		y = getH()/s->h/2;
	else if(y > 1-getH()/s->h/2)
		y = 1-getH()/s->h/2;
	
	if (resolve)
	{
		const string& str = names[label];
		text.render(s, 	str.c_str(), (int)(s->w * x) - text.getW()*str.length()/2, (int)(s->h*y) - text.getH()/2 , fadeval, color);
	}
	else
	{
		char b[16];
		longtoip(b, 16, ntohl(label));
		text.render(s, b, (int)(s->w * x) - text.getW()*strlen(b)/2, (int)(s->h*y) - text.getH()/2 , fadeval, color);
	}
	return true;
}
		
//move the current position and stop the momentum
void entity::jump(float nx, float ny)
{
	x = nx;
	y = ny;
	ndx = 0;
	ndy = 0;
}

