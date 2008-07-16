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

