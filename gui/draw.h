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
		SDL_LockSurface(s);
		int ymin = y1 < 0 ? 0 : y1;
		int ymax = y2 > s->h ? s->h : y2;
		Uint8* p = (Uint8*)s->pixels + ymin * s->pitch + ((x) << 2);
		for(;ymin < ymax;ymin++, p+=s->pitch)
			*(Uint32*)p = c;
		SDL_UnlockSurface(s);
	}
	inline void horizLine(int x1, int x2, int y, Uint32 c, SDL_Surface* s)
	{
		SDL_LockSurface(s);
		int xmin = x1 < 0 ? 0 : x1;
		int xmax = x2 > s->w ? s->w : x2;
		Uint8* p = (Uint8*)s->pixels + y * s->pitch + (xmin << 2);
		for(;xmin < xmax;xmin++, p+=4)
			*(Uint32*)p = c;
		SDL_UnlockSurface(s);
	}
	inline void box(SDL_Rect* r, Uint32 c, SDL_Surface*s)
	{
		vertLine(r->x, r->y, r->y + r->h, c, s);
		vertLine(r->x+r->w-1, r->y, r->y + r->h, c, s);
		horizLine(r->x, r->x + r->w, r->y, c, s);
		horizLine(r->x, r->x + r->w, r->y+ r->h-1, c, s);
	}
};

#endif//DRAW_H
