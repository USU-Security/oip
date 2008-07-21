#include "button.h"

namespace gui
{
	void button::draw(int x, int y, SDL_Surface* s)
	{
		if (clicked) //have it drawn with the alternate image
		{
			SDL_Surface* old = bg;
			bg = imgdown;
			px = py = clickpush;
			label::draw(x, y, s);
			bg = old;
		}
		else
		{
			px = py = 0;
			label::draw(x, y, s);
		}
	}
	bool button::keydown(SDL_KeyboardEvent& k)
	{
		if (!clicked && (k.keysym.sym == SDLK_SPACE || k.keysym.sym == SDLK_RETURN))
		{
			clicked = true;
			downkey = k.keysym.sym;
			return true;
		}
		return false;
	}
	bool button::keyup(SDL_KeyboardEvent&k)
	{
		if (clicked && k.keysym.sym == downkey)
		{
			clicked = false;
			clickit(clicked);
			return true;
		}
		return false;
	}

};
