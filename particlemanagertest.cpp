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
	const char* nodes[] = {
		"10.0.0.1","10.0.0.2","10.0.0.3","10.0.0.4","10.0.0.5",
		"10.0.0.6","10.0.0.7","10.0.0.8","10.0.0.9","10.0.0.10"
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
				if (event.key.keysym.sym == SDLK_SPACE)
					pm.packet(string(nodes[rand()%10]), string(nodes[rand()%10]), 1);	

				break;
			}
		}
		int which = rand()%13;
		pm.packet(string(nodes[pairs[which][0]]), string(nodes[pairs[which][1]]), .01);	
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
