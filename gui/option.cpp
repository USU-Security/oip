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
			activate();
			return true;
		}
		return false;
	}
	bool option::mousedown(SDL_MouseButtonEvent & m)
	{
		if (m.button == SDL_BUTTON_LEFT)
		{
			state = true;
			downkey = SDLK_0;
			return true;
		}
		return false;
	}
	bool option::mouseup(SDL_MouseButtonEvent & m)
	{
		if (m.button == SDL_BUTTON_LEFT && state)
		{
			state = false;
			if (m.x < w && m.y < h) //if within bounds, activate
				activate();
			return true;
		}
		return false;
	}
	void option::activate() 
	{
		clicked = !clicked;
		clickit(clicked, arg);
	}
};
