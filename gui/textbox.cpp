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
			//all this code to draw a vertical line. maybe it should be its own routine.
			/*
			SDL_LockSurface(s);
			int ymin = y < 0 ? 0 : y;
			int ymax = y + h > s->h ? s->h : y+h;
			Uint8* p = (Uint8*)s->pixels + ymin * s->pitch + ((x + nw) << 2);
			for(;ymin < ymax;ymin++, p+=s->pitch)
				*(Uint32*)p = f.getColor();
			SDL_UnlockSurface(s);
			*/
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
			if (isprint(k.keysym.sym))
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
			break;
		}
		return false;
	}
	bool textbox::keyup(SDL_KeyboardEvent&k)
	{
		return false;
	}


};
