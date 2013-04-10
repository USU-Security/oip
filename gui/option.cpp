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
#include "option.h"

namespace gui
{
	bool option::keydown(SDL_KeyboardEvent& k)
	{
		if (!state && (k.keysym.sym == SDLK_SPACE || k.keysym.sym == SDLK_RETURN))
		{
			state = true;
			downkey = k.keysym.sym;
			return true;
		}
		return false;
	}
	bool option::keyup(SDL_KeyboardEvent&k)
	{
		if (state && k.keysym.sym == downkey)
		{
			state = false;
			activate();
			return true;
		}
		return false;
	}
	bool option::mousedown(SDL_MouseButtonEvent & m)
	{
		if (m.button == SDL_BUTTON_LEFT)
		{
			state = true;
			downkey = SDLK_0;
			return true;
		}
		return false;
	}
	bool option::mouseup(SDL_MouseButtonEvent & m)
	{
		if (m.button == SDL_BUTTON_LEFT && state)
		{
			state = false;
			if (m.x < w && m.y < h) //if within bounds, activate
				activate();
			return true;
		}
		return false;
	}
	void option::activate() 
	{
		clicked = !clicked;
		clickit(clicked, arg);
	}
};
