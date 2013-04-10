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
/*
 * implements a textbox
 */

#include "textbox.h"
#include <iostream>
#include "draw.h"
using std::cout;

/* the rate of blinking in ms */
#define BLINKRATE 500

namespace gui
{
	void textbox::draw(int x, int y, SDL_Surface*s)
	{
		//we need to move the text over if the width is too long
		int nw,h2;
		f.getSize(txt.substr(0, insertpos), nw, h2);
		if (nw > .8 * w)
		{
			offset = nw - .8 * w;
			nw = .8 * w;
		}
		else
			offset = 0;
		label::draw(x,y,s);
		int now = SDL_GetTicks();
		if (hasfocus && (lastblink + BLINKRATE > now) && (x+nw > 0 && x+nw < s->w))
		{
			if (!offset)
				vertLine(x+rx+nw, y + ry - f.getSize(), y + ry+2 , f.getColor(), s);
			else
				vertLine(x + nw, y + ry - f.getSize(), y + ry+2 , f.getColor(), s);
		}
		else if (lastblink + BLINKRATE*2 < now)
			lastblink = SDL_GetTicks();
	}
	bool textbox::keydown(SDL_KeyboardEvent&k)
	{
		lastblink = SDL_GetTicks();
		switch (k.keysym.sym)
		{
		case SDLK_LEFT:
			if (insertpos <= 0)
			{	
				insertpos = 0;
				return false;
			}
			else
				insertpos--;
			cachevalid = false;
			return true;
			break;
		case SDLK_RIGHT:
			if (insertpos > txt.size())
			{	
				insertpos = txt.size();
				return false;
			}
			else
				insertpos++;
			cachevalid = false;
			return true;
			break;
		case SDLK_BACKSPACE:
			if (insertpos > 0)
			{
				insertpos--;
				txt.erase(insertpos, 1);
				cachevalid = false;
				return true;
			}
			break;
		case SDLK_DELETE:
			if (insertpos < txt.size())
			{
				txt.erase(insertpos, 1);
				cachevalid = false;
				return true;
			}
			break;
		case SDLK_END:
			insertpos = txt.size();
			cachevalid = false;
			return true;
		case SDLK_HOME:
			insertpos = 0;
			cachevalid = false;
			return true;
		case SDLK_TAB:
			return false; //this key changes the focus. not for us
		default:
			if (isprint(k.keysym.unicode))
			{
				/*
				cout << (char)k.keysym.unicode << "\n";
				if (k.keysym.mod & KMOD_SHIFT)
					txt.insert(insertpos, 1, (char)(isalpha(k.keysym.sym)? k.keysym.sym - 32:k.keysym.sym));
				else
					txt.insert(insertpos, 1, (char)k.keysym.sym);
				// */
				txt.insert(insertpos, 1, (char)k.keysym.unicode);
				
				cachevalid = false;
				insertpos++;
				return true;
			}
			else
				cout << k.keysym.sym << "\n";

			break;
		}
		return false;
	}
	bool textbox::keyup(SDL_KeyboardEvent&k)
	{
		return false;
	}
	bool textbox::mousedown(SDL_MouseButtonEvent&m)
	{
		if (m.button == SDL_BUTTON_LEFT) //take the focus if its a left click
		{
			//figure out where the cursor should go
			int x, y;
			for (insertpos = 1; insertpos <= txt.size(); insertpos++)
			{
				f.getSize(txt.substr(0, insertpos), x, y);
				if (x-offset > m.x)
					break;
			}
			lastblink = SDL_GetTicks(); //turn the cursor on
			if (insertpos == txt.size() && x-offset <= m.x);
			else
				insertpos--; //position it before the character
			cachevalid = false; //redraw the textbox
			return true;
		}
#ifdef __linux__
		else if (m.button == SDL_BUTTON_MIDDLE)
		{
			char data[1024];
			FILE* f = popen("xclip -o", "r");
			if (!f)
				cout << "install xclip if you want copy/paste to be functional\n";
			else
			{
				data[fread(data, 1, 1024, f)] = 0;
				pclose(f);
				txt = data;
				cout << "Pasted '" << data << "'\n";
				insertpos = 0;
				cachevalid = false;
				return true;
			}
		}
		else
			cout << "hello\n";
#endif


		
		return false;
	}
	bool textbox::mouseup(SDL_MouseButtonEvent&m)
	{
		return false;
	}


};
