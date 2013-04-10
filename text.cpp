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
#include "text.h"
#include <SDL/SDL_image.h>

_text text(DATADIR "6x13.png", 6, 13);

_text::_text(const char* font, int w, int h)
{
	SDL_Surface*t;
	height = h;
	width = w;
	if (!(t = IMG_Load(font)))
		throw SDL_GetError();
	SDL_SetAlpha(t, SDL_SRCALPHA, 255);
	txt[0xffffffff] = t;
}

_text::~_text()
{
	map<unsigned int, SDL_Surface*>::iterator i;
	for (i = txt.begin(); i != txt.end(); i++)
		SDL_FreeSurface((*i).second);
}

void _text::newcolor(unsigned int c)
{
	SDL_Surface* t = txt[0xffffffff];
	SDL_Surface* n = SDL_CreateRGBSurface(SDL_HWSURFACE, t->w, t->h, 32, 0xff000000,0x00ff0000, 0x0000ff00, 0x000000ff);
	SDL_LockSurface(t);
	SDL_LockSurface(n);
	Uint32 * s = (Uint32*)t->pixels;
	Uint32 * d = (Uint32*)n->pixels;
	int i;
	Uint8 c_r, c_g, c_b, c_a;
	SDL_GetRGBA(c, t->format, &c_r, &c_g, &c_b, &c_a);
	for (i = 0; i < t->w * t->h; i++)
	{
		Uint8 r, g, b, a;
		SDL_GetRGBA(*s, t->format, &r, &g, &b, &a);
		r = c_r * ((float)a/255);
		g = c_g * ((float)a/255);
		b = c_b * ((float)a/255);
		*d = SDL_MapRGBA(n->format, r, g, b, a);
		s++;
		d++;
	}
	SDL_UnlockSurface(n);
	SDL_UnlockSurface(t);
	SDL_SetAlpha(n, SDL_SRCALPHA, 255);
	txt[c] = n;
}

_text& _text::render(SDL_Surface*surf, const char*s, int x, int y, int fade, unsigned int color)
{
	if (txt.find(color) == txt.end())
		newcolor(color);
	SDL_Surface* t = txt[color];
		
	SDL_Rect src, dst;
	int pos = 0;
	src.y = fade*height;
	src.w = width;
	src.h = height;
	if (x < 0)
		x = 0;
	if (x + width >= surf->w)
		x = surf->w - width-1;
	if (y < 0)
		y = 0;
	if (y + height >= surf->h)
		y = surf->h - height-1;
	
	dst.y = y;
	dst.x = x;
	while(*s && *s >= 32 && *s <= 126)
	{
		src.x = (*s - 32) * width;
		SDL_BlitSurface(t, &src, surf, &dst);
		dst.x += width;
		s++;
	}
	//lh = height;
	//lw = dst.x - x;
	SDL_UpdateRect(surf, x, y, dst.x - x, height);
	return *this;
}
