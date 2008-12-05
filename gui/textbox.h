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
#ifndef GUI_TEXTBOX_H
#define GUI_TEXTBOX_H

#include "label.h"

namespace gui
{
	class textbox:public label
	{
	protected:
		int insertpos;
		int lastblink;
		bool hasfocus;
	public:
		textbox(int width, int height):label(width,height),insertpos(0),lastblink(SDL_GetTicks()),hasfocus(false) {SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);SDL_EnableUNICODE(1);}
		textbox(const char* bgfile):label(bgfile),insertpos(0),lastblink(SDL_GetTicks()),hasfocus(false) {SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);SDL_EnableUNICODE(1);}
		
		virtual void draw(int x, int y, SDL_Surface* s);
		virtual bool keyup(SDL_KeyboardEvent & k);
		virtual bool keydown(SDL_KeyboardEvent & k); 
		virtual bool mouseup(SDL_MouseButtonEvent & m);
		virtual bool mousedown(SDL_MouseButtonEvent & m);
		virtual bool mousemove(SDL_MouseMotionEvent & m) { return false; }
		//accept the focus if it is given to us
		virtual bool focus() {hasfocus = true; return true;}
		virtual void unfocus() {hasfocus = false;}

	};
};


#endif//GUI_TEXTBOX_H
