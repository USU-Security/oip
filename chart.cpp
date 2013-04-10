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
#include "chart.h"
#include "text.h"
#include "gui/draw.h"

#include <sstream>
using std::stringstream;

chart::chart(const char* u, Uint32 c)
{
	unit = u;
	color = c;

	//some reasonable defaults
	lastupdate = SDL_GetTicks();
	minmax = 100 * 1024; 
	maxval = minmax;
	tickwidth = 2;
	setTickInterval(FRAMERATE * 4);
	datapos = MAXWIDTH-1;
	for(;datapos>=0; datapos--)
		data[datapos]=0;
	datapos++;
	datalen = 0;
	bg = 0;
	resize(512, 128);
}

void chart::resize(int width, int height)
{
	if (width > MAXWIDTH)
		width = MAXWIDTH;
	w = width;
	h = height;
	if (bg)
		SDL_FreeSurface(bg);
	bg = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 32, 0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
	datalen = width / tickwidth;
	redraw();
}

void chart::redraw()
{
	int i = (datapos) + 1 % datalen;
	int x = 0;
	SDL_FillRect(bg, NULL, 0x000000ff);
	int newmaxval=data[datapos];
	do
	{
		SDL_Rect r = {x, h - data[i] / maxval * h, tickwidth, data[i] / maxval * h};
		SDL_FillRect(bg, &r, color);
		if (data[i] > newmaxval)
			newmaxval = data[i];
		i = (i + 1) % datalen;
		x += tickwidth;
	}
	while (i != datapos);
	maxval = newmaxval;
	drawscale();
}

void chart::scroll()
{
	SDL_Rect dst;
	/* this doesnt seem to work. 
	dst.x = -tickwidth;
	dst.y = 0;
	SDL_BlitSurface(bg, NULL, bg, &dst);
	// */
	datapos = (datapos + 1) % datalen;
	data[datapos] = 0;
	redraw();
	dst.x = w - tickwidth;
	dst.y = 0;
	dst.w = tickwidth;
	dst.h = h;
	SDL_FillRect(bg, &dst, 0);
}

void chart::copypacket(unsigned int src, unsigned int dst, unsigned int color, unsigned int size)
{
	data[datapos] += size;
}

void chart::draw(int x, int y, SDL_Surface* s)
{
	Uint32 now = SDL_GetTicks();
	if (data[datapos] > maxval)
	{
		maxval = data[datapos];
		redraw();
	}
	SDL_Rect r = {w-tickwidth, h - data[datapos] / maxval * h, tickwidth, data[datapos] / maxval * h};
	SDL_FillRect(bg, &r, color);
	if (now > lastupdate + tickinterval)
	{
		scroll();
		lastupdate = now;
	}
	r.x = x;
	r.y = y;
	SDL_BlitSurface(bg, NULL, s, &r);
}

chart::~chart()
{
	if (bg)
		SDL_FreeSurface(bg);
}

void chart::drawscale()
{
//*
	//conver to a per second value
	float maxrate = maxval * spti;
	int i;
	for (i = 1; i < PREFIXCOUNT-1; i++)
		if (maxrate < prefixes[i].value)
			break;
	i--;
	float scalemax = maxrate / prefixes[i].value;

	//draw 2 lines
	float line1 = (int)scalemax;
	if (h - h * (line1/scalemax) < text.getH()) //is there enough room?
		line1 = (int)((float)(h-text.getH()) / h * scalemax);
	float line2 = line1/2;
	gui::horizLine(0, w, h - h * ((line1/scalemax)), 0x404040ff, bg);
	gui::horizLine(0, w, h - h * ((line2/scalemax)), 0x404040ff, bg);
	int tw, th;
	stringstream s, ss;
	s << line1 << " " << prefixes[i].prefix << unit;
	text.getSize(s.str().c_str(), tw, th);
	text.render(bg, s.str().c_str(), w - tw, h - h * ((line1/scalemax))-th, 10);
	ss << line2 << " " << prefixes[i].prefix << unit;
	text.getSize(ss.str().c_str(), tw, th);
	text.render(bg, ss.str().c_str(), w - tw, h - h * ((line2/scalemax))-th,10);
	// */

}
