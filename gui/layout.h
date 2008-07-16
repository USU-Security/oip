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

	enum fadetype {PLAIN, FADE, LEFT, RIGHT, BOTTOM, TOP};
	class layout: public base
	{
	protected:
		vector<layoutchild> children;
		int w, h;
		int which;
		//which way to move the focus. 
		virtual void moveright();
		virtual void moveleft();
		virtual void moveup();
		virtual void movedown();
	public:
		layout():w(0),h(0),which(0) {}

		virtual void draw(int x, int y, SDL_Surface* s);
		virtual bool keyup(SDL_KeyboardEvent & k);
		virtual bool keydown(SDL_KeyboardEvent & k);
		virtual bool focus(); 
		virtual void unfocus() {if (children.size()) children[which].c->unfocus(); }

		virtual void setSize(int width, int height) {w = width; h = height;}
		
		void addchild(base& c, int x, int y) {SDL_Rect p = {x, y, c.getWidth(), c.getHeight()}; 
children.push_back(layoutchild(&c, p));}
	};
};

#endif//GUI_BASE_H
