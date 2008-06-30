#include <sys/time.h>
#include "entity.h"
#include <stdlib.h>

#include "text.h"

#define FADERATE .25
#define UNFADERATE .031250

const float MINMOVE = .0000001;

inline double now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec/1000000.0;
}



void entity::init(const string& l)
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
}

entity::entity(const string& l)
{
	init(l);
}

entity::entity(const char* l)
{
	init(string(l));
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
	
	text.render(s, 	label.c_str(), (int)(s->w * x) - text.getW()*label.length()/2, (int)(s->h*y) - text.getH()/2 , fadeval);
}
		
//move the current position and stop the momentum
void entity::jump(float nx, float ny)
{
	x = nx;
	y = ny;
	ndx = 0;
	ndy = 0;
}

