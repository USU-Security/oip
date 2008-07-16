#include "widget.h"

namespace gui
{
	void widget::draw(int x, int y, SDL_Surface* s)
	{
		if (bg)
		{
			SDL_Rect dst;
			dst.y = y;
			dst.x = x;
			SDL_BlitSurface(bg, NULL, s, &dst);
		}
	}
};

