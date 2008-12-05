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

particle::particle(float nx, float ny, float nsize, unsigned int ncolor, const string& s): x(nx),y(ny),size(nsize),color(ncolor),dst(s),dx(0),dy(0)
{}

#define PIXEL(s, x, y) (*((Uint32*)(s)->pixels + (Uint32)(y)*(s)->w + (Uint32)(x)))

void particle::draw(SDL_Surface*s)
{
	float xstep = 1.0/s->w;
	float ystep = 1.0/s->h;
	float total=0;
	float x0 = x > size? -size : -x;
	float x1 = x < 1-size? size : 1-x;
	float y0 = y > size? -size : -y;
	float y1 = y < 1-size? size : 1-y;
	for (float i = x0 ; i < x1; i+=xstep)
	{
		for (float j = y0; j < y1; j+=ystep)
		{
			float aa = 0;
			//simple filter, test four coordinates in the square to see if they are in the circle
			if ((i + xstep) * (i + xstep) + (j + ystep) * (j + ystep) < size * size)
				aa += .25;
			if ((i - xstep) * (i - xstep) + (j + ystep) * (j + ystep) < size * size)
				aa += .25;
			if ((i + xstep) * (i + xstep) + (j - ystep) * (j - ystep) < size * size)
				aa += .25;
			if ((i - xstep) * (i - xstep) + (j - ystep) * (j - ystep) < size * size)
				aa += .25;
			total += aa;
			Uint8 r,g,b;
			Uint8 nr,ng,nb;
			SDL_GetRGB(PIXEL(s, (x + i)*s->w, (y+j)*s->h), s->format, &r, &g, &b);
			SDL_GetRGB(color, s->format, &nr, &ng, &nb);
			if (nr * aa + r > 255)
				r = 255;
			else
				r = nr * aa + r;
			if (ng * aa + g > 255)
				g = 255;
			else
				g = ng * aa + g;
			if (nb * aa + b > 255)
				b = 255;
			else
				b = nb * aa + b;
			
			PIXEL(s, (x + i) * s->w , (y+j)*s->h ) = SDL_MapRGB(s->format, r, g, b);

		}
	}
	//if no pixels have been set, set a bare minimum
	if (total < .24) 
	{
		float aa = .25;
		Uint8 r,g,b;
		Uint8 nr,ng,nb;
		SDL_GetRGB(PIXEL(s, (x )*s->w, (y)*s->h), s->format, &r, &g, &b);
		SDL_GetRGB(color, s->format, &nr, &ng, &nb);
		if (nr * aa + r > 255)
			r = 255;
		else
			r = nr * aa + r;
		if (ng * aa + g > 255)
			g = 255;
		else
			g = ng * aa + g;
		if (nb * aa + b > 255)
			b = 255;
		else
			b = nb * aa + b;
		
		PIXEL(s, (x ) * s->w , (y)*s->h ) = SDL_MapRGB(s->format, r, g, b);
	}
}
