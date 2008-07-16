#include "layout.h"
#include <iostream>
using std::cout;
#include "draw.h"

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

	void layout::draw(int x, int y, SDL_Surface* s)
	{
		vector<layoutchild>::iterator i;
		int c=0;
		for (i = children.begin(); i != children.end(); ++i, c++)
		{
			if (c == which)
			{
				SDL_Rect r = (*i).pos;
				r.x-=5;r.y-=5;r.w+=10;r.h+=10;
				//SDL_FillRect(s, &r, SDL_MapRGB(s->format, 0,0,32));
				box(&r, SDL_MapRGB(s->format, 0, 0, 32), s);
				r.x++;r.y++;r.w-=2;r.h-=2;
				//SDL_FillRect(s, &r, SDL_MapRGB(s->format, 0,0,64));
				box(&r, SDL_MapRGB(s->format, 0, 0, 64), s);
				r.x++;r.y++;r.w-=2;r.h-=2;
				//SDL_FillRect(s, &r, SDL_MapRGB(s->format, 0,0,96));
				box(&r, SDL_MapRGB(s->format, 0, 0, 96), s);
				r.x++;r.y++;r.w-=2;r.h-=2;
				//SDL_FillRect(s, &r, SDL_MapRGB(s->format, 0,0,128));
				box(&r, SDL_MapRGB(s->format, 0, 0, 64), s);
				r.x++;r.y++;r.w-=2;r.h-=2;
				//SDL_FillRect(s, &r, SDL_MapRGB(s->format, 0,0,128));
				box(&r, SDL_MapRGB(s->format, 0, 0, 32), s);
			}
			(*i).c->draw((*i).pos.x + x, (*i).pos.y+y, s);
			
		}
	}

	bool layout::keydown(SDL_KeyboardEvent&k)
	{
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
		if (children.size())
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


};
