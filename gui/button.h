#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include <SDL/SDL.h>
#include "label.h"

namespace gui
{
	typedef void (*clickevent)(bool down);
	class button:public label
	{
	protected:
		SDL_Surface * imgdown;
		bool clicked;
		clickevent clickit;
		SDLKey downkey;
		int clickpush;
	public:
		button(const char* upimg, const char* downimg, clickevent c, int o=5):label(upimg),imgdown(image(downimg)),clicked(false),clickit(c),clickpush(o) {centerH(true);centerV(true);}
		~button() {}


		virtual void draw(int x, int y, SDL_Surface* s);
		virtual bool keyup(SDL_KeyboardEvent& k);
		virtual bool keydown(SDL_KeyboardEvent&k);
		virtual bool focus() {return true;}
		virtual void unfocus() {clicked = false;}
	};
};


#endif//GUI_LABEL_H
