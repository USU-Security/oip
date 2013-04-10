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
#ifndef GUI_LAYOUT_H
#define GUI_LAYOUT_H

#include "base.h"

#include <vector>
using std::vector;



namespace gui
{
	struct layoutchild
	{
	public:
		layoutchild(base*b, SDL_Rect& r):c(b),pos(r) {}
		base* c;
		SDL_Rect pos;

	};

	enum fadetype {NONE, PLAIN, FADE, LEFT, RIGHT, BOTTOM, TOP};
	class layout: public base
	{
	protected:
		vector<layoutchild> children;
		float dx, dy;
		int which;
		fadetype fading;
		bool showing;
		Uint32 ti;
		bool focused;
		//which way to move the focus. 
		virtual bool moveright();
		virtual bool moveleft();
		virtual bool moveup();
		virtual bool movedown();
		void dofade(int x, int y, const SDL_Surface* s);
		bool mousestate[3];
	public:
		layout():which(0),dx(0),dy(0),fading(NONE),focused(false),hasborder(0) {selhilite=false;mousestate[0]=mousestate[1]=mousestate[2]=false;}

		virtual void draw(int x, int y, SDL_Surface* s);
		virtual bool keyup(SDL_KeyboardEvent & k);
		virtual bool keydown(SDL_KeyboardEvent & k);
		virtual bool mouseup(SDL_MouseButtonEvent & m);
		virtual bool mousedown(SDL_MouseButtonEvent & m);
		virtual bool mousemove(SDL_MouseMotionEvent & m) { return false; }

		virtual bool focus(); 
		virtual void unfocus() {if (children.size()) children[which].c->unfocus(); focused = false;}

		virtual void setSize(int width, int height) {w = width; h = height;}
		
		void addchild(base& c, int x, int y); 

		void hide(fadetype f=PLAIN);
		void show(fadetype f=PLAIN);
		bool shown() { return visible;}
		Uint32 hasborder; //could stick the color in here
	};
};

#endif//GUI_BASE_H
