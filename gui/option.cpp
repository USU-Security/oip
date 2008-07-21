#include "option.h"

namespace gui
{
	bool option::keydown(SDL_KeyboardEvent& k)
	{
		if (!state && (k.keysym.sym == SDLK_SPACE || k.keysym.sym == SDLK_RETURN))
		{
			state = true;
			downkey = k.keysym.sym;
			return true;
		}
		return false;
	}
	bool option::keyup(SDL_KeyboardEvent&k)
	{
		if (state && k.keysym.sym == downkey)
		{
			state = false;
			clicked = !clicked;
			clickit(clicked);
			return true;
		}
		return false;
	}
};
