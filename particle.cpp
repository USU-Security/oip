
#include "particle.h"
#include "image.h"
#include <iostream>


#define PSIZE 80
#define PCOUNT 20
using std::cout;
particle::particle(float nx, float ny, float nsize, unsigned int ncolor, unsigned int s): x(nx),y(ny),color(ncolor),dst(s),dx(0),dy(0),speed(.15),damp(.97),softdelete(false),invalid(false)
{
	size = (int)(nsize * PCOUNT);
	if (size >= PCOUNT)
		size = PCOUNT-1;
}

#define PIXEL(s, x, y) (*((Uint32*)(s)->pixels + (Uint32)(y)*(s)->w + (Uint32)(x)))

void particle::draw(SDL_Surface*s)
{
	//compute the new position
	move();

	//grab the available images
	SDL_Surface* circles = image("particles.png");
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

