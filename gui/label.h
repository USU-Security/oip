#ifndef GUI_LABEL_H
#define GUI_LABEL_H

#include <SDL/SDL.h>
#include "widget.h"
#include "../image.h"
#include "font.h"

namespace gui
{
	class label:public widget 
	{
	protected:
		string txt;
		font f;
		SDL_Surface* cached;
		bool cachevalid;
		int offset;
		int px, py;
		bool centerx, centery;
		int rx, ry; //the offset where the text was actually rendered
	public:
		label(int width, int height):widget(width,height),cached(NULL),cachevalid(false),offset(0),centerx(false),centery(false),px(0),py(0) {}
		label(const char* bgfile):widget(bgfile),cached(NULL),cachevalid(false),offset(0),centerx(false),centery(false),px(0),py(0) {}
		~label() {if (cached) SDL_FreeSurface(cached);}

		void setString(const string& s) { txt = s; cachevalid = false; }
		const string& getString() { return txt; }
		void setFont(const font& newfont) {f = newfont; cachevalid = false; }

		virtual void draw(int x, int y, SDL_Surface* s);

		virtual void setSize(int width, int height) {w = width; h = height; SDL_FreeSurface(cached); cached = NULL;}
		void centerH(bool c) {centerx = c; cachevalid=false; }
		void centerV(bool c) {centery = c; cachevalid=false; }
	};
};


#endif//GUI_LABEL_H
