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

