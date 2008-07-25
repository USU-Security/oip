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
