
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
#define FADERATE .25
#define UNFADERATE .031250

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
}

entity::entity(unsigned int l)
{
	net = false;
	init(l);
}
// handle fading
void entity::dofade(double dt)
{
	if (fade && fadeval < 15 && lastupdate + FADERATE < now() && moving)
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
}

bool entity::draw(SDL_Surface* s)
{
	x += ndx;
	y += ndy;
	int width;
	char tmp[32];
	if (net)
	{
		longtoip(tmp, 16, ntohl(label));
		snprintf(tmp + strlen(tmp), 32 - strlen(tmp), "/%d (%d)", (int)mask, count);
		width = strlen(tmp) * text.getW();
	}
	else
		width = getW();
		
		
	if (x < width/s->w/2) 
		x = width/s->w/2;
	else if (x > 1-width/s->w/2)
		x = 1-width/s->w/2;
	if (y < getH()/s->h/2)
		y = getH()/s->h/2;
	else if(y > 1-getH()/s->h/2)
		y = 1-getH()/s->h/2;
	if (net)
	{
		text.render(s, tmp, (int)(s->w * x) - width/2, (int)(s->h*y) - text.getH()/2, fadeval);
	}
	else if (resolve)
	{
		const string& str = names[label];
		text.render(s, 	str.c_str(), (int)(s->w * x) - text.getW()*str.length()/2, (int)(s->h*y) - text.getH()/2 , fadeval);
	}
	else
	{
		char b[16];
		longtoip(b, 16, ntohl(label));
		text.render(s, b, (int)(s->w * x) - text.getW()*strlen(b)/2, (int)(s->h*y) - text.getH()/2 , fadeval);
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

