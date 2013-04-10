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
#include "textbox.h"
#include "font.h"
#include "layout.h"
#include "button.h"
#include "option.h"
#define frand ((float)rand()/RAND_MAX)
inline double now()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec/1000000.0;
}

void btnPressed(bool state, void* args)
{
	cout << "Button was pressed\n";
}
void slnPressed(bool state, void* args)
{
	if (state)
		cout << "selection is now up\n";
	else
		cout << "selection is now down\n";
}
int main()
{
	bool visible = true;
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
	gui::label label1(DATADIR "lblbg.png");
	gui::textbox text1(300, 60);
	gui::textbox text2(DATADIR "text2.png");
	gui::button btn1(DATADIR "btnup.png", DATADIR "btndown.png", btnPressed);
	btn1.setString("This is a button");
	label1.setString("This is a string");
	label1.setFont(f2);
	text1.setString("This is a string");
	text2.setFont(f2);
	gui::option opn1(DATADIR "mnuup.png", DATADIR "mnudn.png", slnPressed);
	gui::layout wset;
	wset.addchild(label1, 0,0);
	wset.addchild(text1, 0, 50);
	wset.addchild(text2, 0, 100);
	wset.addchild(btn1, 0, 150);
	wset.addchild(opn1, 0, 200);
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
				else if (event.key.keysym.sym == SDLK_F9)
				{
					if (visible)
						wset.hide(gui::LEFT);	
					else
						wset.show(gui::LEFT);
					visible = !visible;
				}
				else if (event.key.keysym.sym == SDLK_F10)
				{
					if (visible)
						wset.hide(gui::TOP);	
					else
						wset.show(gui::TOP);
					visible = !visible;
				}
				else if (event.key.keysym.sym == SDLK_F11)
				{
					if (visible)
						wset.hide(gui::RIGHT);	
					else
						wset.show(gui::RIGHT);
					visible = !visible;
				}
				else if (event.key.keysym.sym == SDLK_F12)
				{
					if (visible)
						wset.hide(gui::BOTTOM);	
					else
						wset.show(gui::BOTTOM);
					visible = !visible;
				}
				else
					wset.keydown(event.key);
				break;
			case SDL_KEYUP:
				wset.keyup(event.key);
				break;
			case SDL_MOUSEBUTTONUP:
				event.button.x -= 100;
				event.button.y -= 50;
				wset.mouseup(event.button);
				break;
			case SDL_MOUSEBUTTONDOWN:
				event.button.x -= 100;
				event.button.y -= 50;
				wset.mousedown(event.button);
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
		wset.draw(100,50,screen);
		f.render("Hello World", -10, 256, screen);
		f.render("Hello World", 500, 256, screen);
		f.render("Hello World", 256, 10, screen);
		f.render("Hello World", 256, 522, screen);
		SDL_Flip(screen);

	}

	return 0;
}
