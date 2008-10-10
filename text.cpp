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

_text text("6x13.png", 6, 13);

_text::_text(const char* font, int w, int h)
{
	height = h;
	width = w;
	if (!(txt = IMG_Load(font)))
		throw SDL_GetError();
	SDL_SetAlpha(txt, SDL_SRCALPHA, 255);
}

_text& _text::render(SDL_Surface*surf, const char*s, int x, int y, int fade, bool alt)
{
	SDL_Rect src, dst;
	int pos = 0;
	src.y = fade*height + (alt ? 208 : 0);
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
		SDL_BlitSurface(txt, &src, surf, &dst);
		dst.x += width;
		s++;
	}
	//lh = height;
	//lw = dst.x - x;
	SDL_UpdateRect(surf, x, y, dst.x - x, height);
	return *this;
}
