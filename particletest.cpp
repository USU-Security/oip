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
#include "particle.h"
#include "kdtree.h"

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

	vector<particle*> particles;
	int i;
	for (i = 0; i < 1000; i++)
		particles.push_back(new particle(frand, frand, frand*.1, SDL_MapRGB(screen->format, frand*255, frand*255, frand*255), string("")));
	
	kdtree * pfind;
	vector<particle*> near;
	bool run=true;
	while(run)
	{
		SDL_Event event;
		double ti = now();
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
				if (event.key.keysym.sym == SDLK_SPACE)
					particles.push_back(new particle(frand, frand, frand*.02, SDL_MapRGB(screen->format, frand*255, frand*255, frand*255), string("")));
					
				break;
			}
		}

		SDL_FillRect(screen, NULL, 0);
		pfind = new kdtree(particles);
		for (i = 0; i < particles.size(); i++)
		{
			pfind->collect(.01, particles[i]->getX(), particles[i]->getY(), near);
			particles[i]->draw(screen);
		}
		delete pfind;
		ti = now() - ti;
		char s[128];
		snprintf(s, 128, "%g fps", 1.0/ti);
		text.render(screen, s, 0, 0);
		SDL_Flip(screen);

	}

	return 0;
}
