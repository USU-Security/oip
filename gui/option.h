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
#ifndef GUI_OPTION_H
#define GUI_OPTION_H

#include "button.h"

namespace gui
{
	class option: public button
	{
	private:
		bool state;
	public:
		option(const char* upimg, const char* downimg, clickevent c, void* a=NULL, int o=5):button(upimg, downimg, c,a,o),state(false) {}


		virtual bool keyup(SDL_KeyboardEvent& k);
		virtual bool keydown(SDL_KeyboardEvent&k);
		virtual bool mouseup(SDL_MouseButtonEvent & m);
		virtual bool mousedown(SDL_MouseButtonEvent & m);
		virtual void unfocus(){}

		virtual void activate();
		void setstate(bool s) {clicked = s;}
	};
};


#endif//GUI_LABEL_H
