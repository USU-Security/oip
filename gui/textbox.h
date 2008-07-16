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
		textbox(int width, int height):label(w,h),insertpos(0),lastblink(SDL_GetTicks()),hasfocus(false) {SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);SDL_EnableUNICODE(1);}
		textbox(const char* bgfile):label(bgfile),insertpos(0),lastblink(SDL_GetTicks()),hasfocus(false) {SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);SDL_EnableUNICODE(1);}
		
		virtual void draw(int x, int y, SDL_Surface* s);
		virtual bool keyup(SDL_KeyboardEvent & k);
		virtual bool keydown(SDL_KeyboardEvent & k); 
		//accept the focus if it is given to us
		virtual bool focus() {hasfocus = true; return true;}
		virtual void unfocus() {hasfocus = false;}

	};
};


#endif//GUI_TEXTBOX_H
