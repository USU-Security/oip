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
