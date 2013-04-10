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
#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include <SDL/SDL.h>
#include "label.h"

namespace gui
{
	typedef void (*clickevent)(bool down, void* arg);
	class button:public label
	{
	protected:
		SDL_Surface * imgdown;
		bool clicked;
		clickevent clickit;
		SDLKey downkey;
		int clickpush;
	public:
		button(const char* upimg, const char* downimg, clickevent c, void* a=NULL, int o=5):label(upimg),imgdown(image(downimg)),clicked(false),clickit(c),clickpush(o),arg(a) {centerH(true);centerV(true);}
		~button() {}


		virtual void draw(int x, int y, SDL_Surface* s);
		virtual bool keyup(SDL_KeyboardEvent& k);
		virtual bool keydown(SDL_KeyboardEvent&k);
		virtual bool mouseup(SDL_MouseButtonEvent & m);
		virtual bool mousedown(SDL_MouseButtonEvent & m);
		virtual bool focus() {return true;}
		virtual void unfocus() {clicked = false;}

		virtual void activate() {clickit(clicked, arg);}
		//so we can change the argument later
		void* arg;
	};
};


#endif//GUI_LABEL_H
