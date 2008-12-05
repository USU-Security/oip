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
#ifndef GUI_BASE_H
#define GUI_BASE_H

#include <SDL/SDL.h>

namespace gui
{
	class base
	{
	protected:
		int w, h;
		bool visible;
		bool selhilite;
	public:
		base():w(0),h(0),visible(true),selhilite(true) {}
		/* called when the object needs to be drawn */
		virtual void draw(int x, int y, SDL_Surface* s) = 0;
		/* called when a keystroke happens. returns whether or not the
		 * object used the keystroke 
		 */
		virtual bool keyup(SDL_KeyboardEvent & k) { return false; }
		virtual bool keydown(SDL_KeyboardEvent & k) { return false; }
		/*
		 * called when a mouseevent happens, returns whether it was used
		 */
		virtual bool mouseup(SDL_MouseButtonEvent & m) {return false; }
		virtual bool mousedown(SDL_MouseButtonEvent & m) {return false; }
		virtual bool mousemove(SDL_MouseMotionEvent & m) {return false; }

		/* calls when object is offered the keyboard events. returns true
		 * to accept them 
		 */
		virtual bool focus() {return false;}
		/* notification that the object will no longer recieve keyboard events*/
		virtual void unfocus() {}

		/* change the size of the widget */
		virtual void setSize(int width, int height) {w = width; h = height;}

		int getWidth() { return w;}
		int getHeight() { return h;}
		bool highlightEnabled() { return selhilite; }
	};
};

#endif//GUI_BASE_H
