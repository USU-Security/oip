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
/*
 * this file contains basic functions for doing drawing
 * TODO: implement alpha blending
 * TODO: remove assumption of 32 bit color
 */

#ifndef DRAW_H
#define DRAW_H

#include <SDL/SDL.h>

namespace gui
{
	inline void vertLine(int x, int y1, int y2, Uint32 c, SDL_Surface* s)
	{
		if (x > 0 && x < s->w)
		{
			SDL_LockSurface(s);
			int ymin = y1 < 0 ? 0 : y1;
			int ymax = y2 > s->h ? s->h : y2;
			Uint8* p = (Uint8*)s->pixels + ymin * s->pitch + ((x) << 2);
			for(;ymin < ymax;ymin++, p+=s->pitch)
				*(Uint32*)p = c;
			SDL_UnlockSurface(s);
		}
	}
	inline void horizLine(int x1, int x2, int y, Uint32 c, SDL_Surface* s)
	{
		if (y > 0 && y < s->h)
		{
			SDL_LockSurface(s);
			int xmin = x1 < 0 ? 0 : x1;
			int xmax = x2 > s->w ? s->w : x2;
			Uint8* p = (Uint8*)s->pixels + y * s->pitch + (xmin << 2);
			for(;xmin < xmax;xmin++, p+=4)
				*(Uint32*)p = c;
			SDL_UnlockSurface(s);
		}
	}
	inline void box(SDL_Rect* r, Uint32 c, SDL_Surface*s)
	{
		vertLine(r->x, r->y, r->y + r->h, c, s);
		vertLine(r->x+r->w-1, r->y, r->y + r->h, c, s);
		horizLine(r->x, r->x + r->w, r->y, c, s);
		horizLine(r->x, r->x + r->w, r->y+ r->h-1, c, s);
	}
	inline void border(SDL_Rect& r, SDL_Surface*s)
	{
		r.x-=5;r.y-=5;r.w+=10;r.h+=10;
		box(&r, SDL_MapRGB(s->format, 0, 0, 64), s);
		r.x++;r.y++;r.w-=2;r.h-=2;
		box(&r, SDL_MapRGB(s->format, 0, 0, 128), s);
		r.x++;r.y++;r.w-=2;r.h-=2;
		box(&r, SDL_MapRGB(s->format, 0, 0, 192), s);
		r.x++;r.y++;r.w-=2;r.h-=2;
		box(&r, SDL_MapRGB(s->format, 0, 0, 128), s);
		r.x++;r.y++;r.w-=2;r.h-=2;
		box(&r, SDL_MapRGB(s->format, 0, 0, 64), s);
	}

};

#endif//DRAW_H
