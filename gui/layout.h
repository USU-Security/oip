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
		int w, h;
		float dx, dy;
		int which;
		fadetype fading;
		bool showing;
		Uint32 ti;
		//which way to move the focus. 
		virtual void moveright();
		virtual void moveleft();
		virtual void moveup();
		virtual void movedown();
		void dofade(int x, int y, const SDL_Surface* s);
	public:
		layout():w(0),h(0),which(0),dx(0),dy(0),fading(NONE) {}

		virtual void draw(int x, int y, SDL_Surface* s);
		virtual bool keyup(SDL_KeyboardEvent & k);
		virtual bool keydown(SDL_KeyboardEvent & k);
		virtual bool focus(); 
		virtual void unfocus() {if (children.size()) children[which].c->unfocus(); }

		virtual void setSize(int width, int height) {w = width; h = height;}
		
		void addchild(base& c, int x, int y); 

		void hide(fadetype f=PLAIN);
		void show(fadetype f=PLAIN);
	};
};

#endif//GUI_BASE_H
