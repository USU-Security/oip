#include "layout.h"
#include <iostream>
using std::cout;
#include "draw.h"

//the rate of movement for disappearing layouts
//defined in pixels per millisecond
#define FADERATE 4

namespace gui
{
	void layout::moveright()
	{
		if (children.size())
		{
			children[which].c->unfocus();
			which = (which + 1) % children.size();
			//find a child willing to accept which
			int old = which;
			do
			{
				if (children[which].c->focus())
					break;
				which = (which + 1) % children.size();
			}
			while (which != old); //it will eventually wrap to where we started
		}
	}
	void layout::movedown()
	{
		if (children.size()) 
		{
			children[which].c->unfocus();
			which = (which + 1) % children.size();
			//find a child willing to accept which
			int old = which;
			do
			{
				if (children[which].c->focus())
					break;
				which = (which + 1) % children.size();
			}
			while (which != old); //it will eventually wrap to where we started
		}
	}
	void layout::moveleft()
	{
		if (children.size()) 
		{
			children[which].c->unfocus();
			which = (which + children.size() - 1) % children.size();
			//find a child willing to accept which
			int old = which;
			do
			{
				if (children[which].c->focus())
					break;
				which = (which + children.size() - 1) % children.size();
			}
			while (which != old); //it will eventually wrap to where we started
		}
	}
	void layout::moveup()
	{
		if (children.size()) 
		{
			children[which].c->unfocus();
			which = (which + children.size() - 1) % children.size();
			//find a child willing to accept which
			int old = which;
			do
			{
				if (children[which].c->focus())
					break;
				which = (which + children.size() - 1) % children.size();
			}
			while (which != old); //it will eventually wrap to where we started
		}
	}

	void layout::dofade(int x, int y, const SDL_Surface* s)
	{
		if (fading && showing && dx == 0 && dy == 0)
		{
			switch (fading)
			{
			case LEFT:
				dx = -w - x;
				break;
			case RIGHT:
				dx = s->w - x;
				break;
			case TOP:
				dy = -h - y;
				break;
			case BOTTOM:
				dy = s->h - y;
				break;
			}
		}
		switch (fading)
		{
		case LEFT:
			if (showing)
			{
				dx += FADERATE * (SDL_GetTicks() - ti);
				if (dx > 0)
				{
					dx = 0;
					fading = NONE;
				}
			}
			else
			{
				dx-= FADERATE * (SDL_GetTicks() - ti);
				if (dx + x + w < 0)
				{
					fading = NONE;
					visible = false;
				}
			}
			break;
		case RIGHT:
			if (showing)
			{
				dx -= FADERATE *  (SDL_GetTicks() - ti);
				if (dx < 0)
				{
					dx = 0;
					fading = NONE;
				}
			}
			else
			{
				dx+= FADERATE *  (SDL_GetTicks() - ti);
				if (dx + x > s->w)
				{
					fading = NONE;
					visible = false;
				}
			}
			break;
		case TOP:
			if (showing)
			{
				dy += FADERATE * (SDL_GetTicks() - ti);
				if (dy > 0)
				{
					fading = NONE;
					dy = 0;
				}
			}
			else
			{
				dy -= FADERATE * (SDL_GetTicks() - ti);
				if (dy + y + h < 0)
				{
					fading = NONE;
					visible = false;
				}
			}
			break;
		case BOTTOM:
			if (showing)
			{
				dy -= FADERATE * (SDL_GetTicks() - ti);
				if (dy < 0)
				{
					fading = NONE;
					dy = 0;
				}
			}
			else
			{
				dy += FADERATE *  (SDL_GetTicks() - ti);
				if (dy + y > s->h)
				{
					fading = NONE;
					visible = false;
				}
			}
			break;
		}
		ti = SDL_GetTicks();
	}

	void layout::draw(int x, int y, SDL_Surface* s)
	{
		//process the fading routines
		dofade(x, y, s);
		if (!visible)
			return;
		
		x += dx;
		y += dy;
		vector<layoutchild>::iterator i;
		int c=0;
		for (i = children.begin(); i != children.end(); ++i, c++)
		{
			if (c == which)
			{
				SDL_Rect r = (*i).pos;
				r.x += x;
				r.y += y;
				r.x-=5;r.y-=5;r.w+=10;r.h+=10;
				//SDL_FillRect(s, &r, SDL_MapRGB(s->format, 0,0,32));
				box(&r, SDL_MapRGB(s->format, 0, 0, 64), s);
				r.x++;r.y++;r.w-=2;r.h-=2;
				//SDL_FillRect(s, &r, SDL_MapRGB(s->format, 0,0,64));
				box(&r, SDL_MapRGB(s->format, 0, 0, 128), s);
				r.x++;r.y++;r.w-=2;r.h-=2;
				//SDL_FillRect(s, &r, SDL_MapRGB(s->format, 0,0,96));
				box(&r, SDL_MapRGB(s->format, 0, 0, 192), s);
				r.x++;r.y++;r.w-=2;r.h-=2;
				//SDL_FillRect(s, &r, SDL_MapRGB(s->format, 0,0,128));
				box(&r, SDL_MapRGB(s->format, 0, 0, 128), s);
				r.x++;r.y++;r.w-=2;r.h-=2;
				//SDL_FillRect(s, &r, SDL_MapRGB(s->format, 0,0,128));
				box(&r, SDL_MapRGB(s->format, 0, 0, 64), s);
			}
			(*i).c->draw((*i).pos.x + x, (*i).pos.y+y, s);
			
		}
	}

	bool layout::keydown(SDL_KeyboardEvent&k)
	{
		if (!visible)
			return false;
		if (children.size())
		{
			//see if the whiched child wants it. 
			if (children[which].c->keydown(k))
				return true;
			//see if the user wants to switch which widget has the which
			switch(k.keysym.sym)
			{
			case SDLK_LEFT:
				moveleft();
				cout << "which is now " << which << "\n";
				break;
			case SDLK_RIGHT:
			case SDLK_TAB:
				moveright();
				cout << "which is now " << which << "\n";
				break;
			case SDLK_DOWN:
				movedown();
				cout << "which is now " << which << "\n";
				break;
			case SDLK_UP:
				moveup();
				cout << "which is now " << which << "\n";
				break;
			default:
				return false;
			}
			return true;
		}
		return false;
	}
	bool layout::keyup(SDL_KeyboardEvent&k)
	{
		if (!visible)
			return false;
		if (children.size())
		{
			//see if the whiched child wants it. 
			if (children[which].c->keyup(k))
				return true;
		}
		return false;
	}
	bool layout::focus()
	{
		if (visible && children.size())
		{
			//find a child willing to accept which
			int old = which;
			do
			{
				if (children[which].c->focus())
					return true;
				which = (which + 1) % children.size();
			}
			while (which != old); //it will eventually wrap to where we started
		}
		return false;
	}
	void layout::hide(fadetype f)
	{
		if (visible && fading == NONE)
		{
			switch (f)
			{
			case TOP:
			case BOTTOM:
			case LEFT:
			case RIGHT:
				dx = dy = 0;
				ti = SDL_GetTicks();
				fading = f;
				showing = false;
				break;
			default:
				visible = false;
			}
		}
	}
	void layout::show(fadetype f)
	{
		if(!visible && fading == NONE)
		{
			visible = true;
			switch (f)
			{
			case TOP:
			case BOTTOM:
			case LEFT:
			case RIGHT:
				showing = true;
				dx = dy = 0;
				ti = SDL_GetTicks();
				fading = f;
				break;
			}
		}
	}
	void layout::addchild(base& c, int x, int y) 
	{
		SDL_Rect p = {x, y, c.getWidth(), c.getHeight()};
		/* if the width is too small, change it to fit */
		if (x + c.getWidth() > w)
			w = x + c.getWidth();
		if (y + c.getHeight() > h)
			h = y + c.getHeight();
		children.push_back(layoutchild(&c, p));
	}


};
