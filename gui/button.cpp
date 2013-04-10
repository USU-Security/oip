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

#include "button.h"

#include <iostream>
using namespace std;

namespace gui
{
	void button::draw(int x, int y, SDL_Surface* s)
	{
		if (clicked) //have it drawn with the alternate image
		{
			SDL_Surface* old = bg;
			bg = imgdown;
			px = py = clickpush;
			label::draw(x, y, s);
			bg = old;
		}
		else
		{
			px = py = 0;
			label::draw(x, y, s);
		}
	}
	bool button::keydown(SDL_KeyboardEvent& k)
	{
		if (!clicked && (k.keysym.sym == SDLK_SPACE || k.keysym.sym == SDLK_RETURN))
		{
			clicked = true;
			downkey = k.keysym.sym;
			return true;
		}
		return false;
	}
	bool button::keyup(SDL_KeyboardEvent&k)
	{
		if (clicked && k.keysym.sym == downkey)
		{
			clicked = false;
			activate();
			return true;
		}
		return false;
	}
	bool button::mousedown(SDL_MouseButtonEvent & m)
	{
		cout << "mouse down\n";
		if (m.button == SDL_BUTTON_LEFT)
		{
			clicked = true;
			downkey = SDLK_0;
			return true;
		}
		return false;
	}
	bool button::mouseup(SDL_MouseButtonEvent & m)
	{
		cout << "mouse up\n";
		if (m.button == SDL_BUTTON_LEFT && clicked)
		{
			clicked = false;
			if (m.x < w && m.y < h) //if within bounds, activate
				activate();
			return true;
		}
		return false;
	}

};
