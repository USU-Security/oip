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
#include "label.h"
#include <iostream>
using std::cout;

//const float sigma1[] = {.006,.061,.242,.383,.242,.061,.006};
//const float kernel[] ={.16,.16,.16,0,.16,.16,.16};
const float kernel[] ={0,.16,.32,0,.32,.16,0};



#define getpixel(s, x, y) *(Uint32*)((Uint8*)s->pixels + (y) * s->pitch + ((x) << 2))
void gaussianblur(SDL_Surface* s)
{
	int i, j, k;
	
	SDL_LockSurface(s);
	//make a copy of the data
	Uint32* data = new Uint32[s->w * s->h];
	memcpy(data, s->pixels, s->w * s->h * sizeof(Uint32));
	//horizontal
	for (j = 0; j < s->h; j++)
	{
		for (i = 0; i < s->w; i++)
		{
			int kmin = i < 4? 3 - i : 0;
			int kmax = i > s->w - 4? s->w - i + 3 : 7;
			Uint8 rt, gt, bt, at;
			rt = gt = bt = at = 0;
			for (k = kmin; k < kmax; k++)
			{
				Uint8 r,g,b,a;
				SDL_GetRGBA(data[j * s->w + i + (k - 3)], s->format, &r, &g, &b, &a);
				rt += r * kernel[k];
				gt += g * kernel[k];
				bt += b * kernel[k];
				at += a * kernel[k];
			}
			getpixel(s, i, j) = SDL_MapRGBA(s->format, rt, gt, bt, at);
		}
	}
	//another copy, smeared horizontally
	memcpy(data, s->pixels, s->w * s->h * sizeof(Uint32));
	//vertical
	for (j = 0; j < s->h; j++)
	{
		for (i = 0; i < s->w; i++)
		{
			int kmin = j < 4? 3 - j : 0;
			int kmax = j > s->h - 4? s->h - j + 3 : 7;
			Uint8 rt, gt, bt, at;
			rt = gt = bt = at = 0;
			for (k = kmin; k < kmax; k++)
			{
				Uint8 r,g,b,a;
				SDL_GetRGBA(data[(j + (k - 3)) * s->w + i], s->format, &r, &g, &b, &a);
				rt += r * kernel[k];
				gt += g * kernel[k];
				bt += b * kernel[k];
				at += a * kernel[k];
			}
			getpixel(s, i, j) = SDL_MapRGBA(s->format, rt, gt, bt, at);
		}
	}
	delete [] data;
	SDL_UnlockSurface(s);
}

namespace gui
{
	void label::draw(int x, int y, SDL_Surface* s)
	{
		if (!cachevalid) //do we need to regenerate the cached image?
		{
			//find the needed size of the cache
			int nw, nh;
			f.getSize(txt, nw, nh);
			if (nw > w) nw = w; //clamp the size to the size of the widget
			if (nh > h) nh = h;
			if (!cached) //does the existing cache need cleared?
			{
				if (!(cached = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_SRCALPHA, w, h, 32, 0xff000000,0x00ff0000,0x0000ff00,0x000000ff)))
					cached = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA, w, h, 32, 0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
			}
			if (!cached)
				cout << "Unable to create cached image: " << SDL_GetError() << "\n";
			SDL_FillRect(cached, NULL, 0); //clear it
			//compute where to render it
			if (!offset && centerx) //if a manual offset has been set, centering doesnt matter
				rx = (w - nw)/2;
			else
				rx = -offset;
			if (centery)
				ry = (h + f.getSize())/2;
			else
				ry = f.getSize();
			Uint32 old = f.getColor();
			Uint8 r,g,b,a;
			SDL_GetRGBA(old, cached->format, &r,&g,&b, &a);
			f.setColor(SDL_MapRGBA(cached->format, ~r, ~g, ~b, a));
			f.render(txt, rx,ry, cached);
			f.setColor(old);
			//apply a gaussian blur to it
			gaussianblur(cached);			
			//render on top of the blur
			f.render(txt, rx,ry, cached);
			cachevalid = true;
		}

		widget::draw(x, y, s);
		SDL_Rect dst;
		dst.x = x+px;
		dst.y = y+py;
		//f.render(txt, x, y+f.getSize(), s);
		SDL_BlitSurface(cached, NULL, s, &dst);
	}
};

