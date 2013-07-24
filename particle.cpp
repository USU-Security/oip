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

#include "particle.h"
#include "image.h"
#include <iostream>
using std::cout;


#define PSIZE 80
#define PCOUNT 20
#define MAXPARTICLEAGE 2
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

particle::particle(float nx, float ny, float nsize, unsigned int ncolor, unsigned int s): x(nx),y(ny),color(ncolor),dst(s),dx(0),dy(0),speed(.15),damp(.97),softdelete(false),invalid(false)
{
	size = (int)(nsize * PCOUNT);
	if (size >= PCOUNT)
		size = PCOUNT-1;
	created = now();
}

#define PIXEL(s, x, y) (*((Uint32*)(s)->pixels + (Uint32)(y)*(s)->w + (Uint32)(x)))

void particle::draw(SDL_Surface*s)
{
	//compute the new position
	move();

	//grab the available images
	SDL_Surface* circles = image(DATADIR "particles-dark-background.png");
	SDL_Rect src, dst;
	src.x = size * PSIZE;//thus valid values for size lie between 0 and 1
	Uint8 r,g,b;
	SDL_GetRGB(color, s->format, &r, &g, &b);
	if (r > g && r > b)
		src.y = 0;
	else if (g > b && g > r)
		src.y = PSIZE;
	else
		src.y = 2*PSIZE;
	src.w = PSIZE;
	src.h = PSIZE;
	dst.x = (int)(x * s->w) - PSIZE/2;
	dst.y = (int)(y * s->h) - PSIZE/2;
	SDL_BlitSurface(circles, &src, s, &dst);
}


/*
 * moves the particle toward the correct destination, while avoiding neighbors
 */
void particle::move(float dstx, float dsty, vector<particle*>& pnear, double dt)
{
	float fx=0, fy=0;
	//force toward destination
	//spring attraction toward destination
	fx -= speed * (x - dstx);
	fy -= speed * (y - dsty);

	//*
	//coulomb repel from nearby particles
	typedef vector<particle*> vp; //another vc++ bug
	vp::iterator i;
	for(i = pnear.begin(); i != pnear.end(); i++)
	{
		if ((*i)->dst == dst) //don't avoid packets heading the same way
			continue;
		float lensq = (
			((*i)->getX()-x)*((*i)->getX()-x) + 
			((*i)->getY()-y)*((*i)->getY()-y))
			*200000	;
		//stuff can get locked into a corner if they are too close
		if (lensq > .00003)
		{
			fx += (x - (*i)->getX())/lensq;
			fy += (y - (*i)->getY())/lensq;
		}
	}
	// */

	dx = (dx + fx * dt) * damp;
	dy = (dy + fy * dt) * damp;

	//if the age is too big, mark for deletion
	if (created + MAXPARTICLEAGE < now())
		softdelete = true;
}

/*
 * called by draw function shortly before drawing actually occurs
 */
void particle::move()
{
	x += dx;
	y += dy;
	if (x < 0)
		x = 0;
	else if (x > 1)
		x = 1;
	if (y < 0)
		y = 0;
	else if (y > 1)
		y = 1;
}

