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
#ifndef CHART_H
#define CHART_H
#include <string>
using std::string;
#include<SDL/SDL.h>
#include "packetpeek.h"
#define PREFIXCOUNT 5
const struct
{
	const char* prefix;
	float value;
} prefixes[PREFIXCOUNT + 1] = {
	{"", 0.0},
	{"K", 1000.0},
	{"M", 1000000.0},
	{"G", 1000000000.0},
	{"T", 1000000000000.0}
};
/*
 * This class displays a histogram
 */
#define FRAMERATE 41
#define MAXWIDTH 2000
class chart:public packetpeek
{
private:
	Uint32 lastupdate;
	Uint32 maxval;
	Uint32 minmax; //the minimum that maxval should be
	Uint32 tickwidth;
	Uint32 tickinterval;
	float spti;
	Uint32 color;
	float data[MAXWIDTH];
	int datalen;
	int datapos;
	int w;
	int h;
	SDL_Surface * bg;
	string unit;

	// used to initialize a surface from the given data. 
	void redraw();
	void scroll();

public:
	//chart(const char* u = "bit", Uint32 c = 0x00005fff);
	chart(const char* u = "bit", Uint32 c = 0x0000ffff);
	~chart();

	void draw(int x, int y, SDL_Surface* s);
	
	void copypacket(unsigned int src, unsigned int dst, unsigned int color, unsigned int size);
	void resize(int width, int height);

	void drawscale();

	void setTickInterval(Uint32 ti) {tickinterval = ti; spti = 1000.0/ti;}
};


#endif//CHART_H
