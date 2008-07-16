/*
 * This file tests the entity manager
 */

#include <iostream>
using namespace std;

#include <sys/time.h>
#include <SDL/SDL.h>
#include <stdlib.h>
#include "textbox.h"
#include "font.h"
#include "layout.h"

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
	
	gui::font f("Vera.ttf", 24);
	gui::font f2;
	f2.setColor(0);
	gui::label label1("lblbg.png");
	gui::textbox text1("lblbg.png");
	gui::textbox text2("lblbg.png");
	label1.setString("This is a string");
	label1.setFont(f2);
	text1.setString("This is a string");
	text1.setFont(f2);
	text2.setFont(f2);
	gui::layout wset;
	wset.addchild(label1, 100,200);
	wset.addchild(text1, 100, 300);
	wset.addchild(text2, 100, 400);
	wset.focus();//allows it to choose a child for focus
	bool run = true;
	double ti=now();
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
				else if (event.key.keysym.sym == SDLK_F1)
					text1.centerH(true);
				else if (event.key.keysym.sym == SDLK_F2)
					text1.centerH(false);
				else if (event.key.keysym.sym == SDLK_F3)
					text1.centerV(true);
				else if (event.key.keysym.sym == SDLK_F4)
					text1.centerV(false);
				else
					wset.keydown(event.key);
				break;
			case SDL_KEYUP:
				wset.keyup(event.key);
				break;
			}
		}
		double dt = now() - ti;
		ti = now();
		SDL_FillRect(screen, NULL, 0);
		char s[128];
		snprintf(s, 128, "%g fps", 1.0/dt);
		//text.render(screen, s, 0, 0);
		SDL_Rect r;
		r.x = 0;
		r.y = 0;
		int w, h;
		f.getSize(s, w, h);
		r.w = w;
		r.h = h;
		SDL_FillRect(screen, &r, 0xff0000ff);
		f.render(s, 0, 24, screen);
		wset.draw(0,0,screen);
		f.render("Hello World", -10, 256, screen);
		f.render("Hello World", 500, 256, screen);
		f.render("Hello World", 256, 10, screen);
		f.render("Hello World", 256, 522, screen);
		SDL_Flip(screen);

	}

	return 0;
}
