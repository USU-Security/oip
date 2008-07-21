#include "label.h"
#include <iostream>
using std::cout;

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

