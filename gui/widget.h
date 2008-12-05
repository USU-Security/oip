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
#ifndef GUI_WIDGET_H
#define GUI_WIDGET_H

#include <SDL/SDL.h>
#include "base.h"
#include "../image.h"

namespace gui
{
	class widget:public base
	{
	protected:
		SDL_Surface * bg;
	public:
		widget(int width, int height):bg(NULL) {setSize(width, height);}
		widget(const char* bgfile) {bg = image(bgfile); w=bg->w; h=bg->h;}
		~widget(){};
		virtual void draw(int x, int y, SDL_Surface* s);
	};
};

#endif//GUI_WIDGET_H

