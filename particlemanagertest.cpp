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
 * This file tests the entity manager
 */

#include <iostream>
using namespace std;

#include <sys/time.h>
#include <SDL/SDL.h>
#include <stdlib.h>
#include "text.h"
#include "particlemanager.h"

#define frand ((float)rand()/RAND_MAX)
inline double now()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec/1000000.0;
}

int main()
{
	if ( SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "Unable to initialize sdl: " << SDL_GetError() << "\n";
		return 1;
	}
	atexit(SDL_Quit);
	SDL_Surface * screen;
	if (!(screen = SDL_SetVideoMode(512,512,32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_RESIZABLE)))
	{
		cout << "Unable to get a hardware surface, falling back on software\n";
		screen = SDL_SetVideoMode(512,512,32, SDL_SWSURFACE|SDL_RESIZABLE);
	}
	if (!screen) 
	{
		cout << "Unable to set display: " << SDL_GetError() << "\n";
		return 2;
	}

	particlemanager pm;
	int i;
	bool run=true;
	double ti = now();
	unsigned int nodes[] = {
		0x0100000a,0x0200000a,0x0300000a,0x0400000a,0x0500000a,
		0x0600000a,0x0700000a,0x0800000a,0x0900000a,0x0a00000a
	};
	const int pairs[][2] = {
		{1, 2},
		{5, 7},
		{7, 1},
		{1, 7},
		{2, 7},
		{0, 7},
		{7, 0},
		{3, 2},
		{4, 2},
		{2, 4},
		{6, 7},
		{8, 9},
		{9, 8}
	};
	while(run)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT: 
				run = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					run = false;

				break;
			}
		}
		int which = rand()%13;
		pm.addpacket(nodes[pairs[which][0]], nodes[pairs[which][1]], 0xff00ff00, 100);	
		double dt = now() - ti;
		ti = now();
		pm.process(dt);
		SDL_FillRect(screen, NULL, 0);
		pm.draw(screen);
		char s[128];
		snprintf(s, 128, "%g fps", 1.0/dt);
		text.render(screen, s, 0, 0);
		SDL_Flip(screen);

	}

	return 0;
}
