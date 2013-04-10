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

#ifndef TEXT_H
#define TEXT_H

#include <SDL/SDL.h>

#include <string.h>
#include <map>
using std::map;
#include <string>
using std::string;

//This class expects to be initialized with a single image, which
//contains ascii characters 32-126, the height and width of each character
class _text 
{
private:
	//the baseline text image
	map<unsigned int, SDL_Surface*> txt;
	int height;
	int width;
	void newcolor(unsigned int c);
public:
	_text(const char* font, int w, int h);
	~_text();
	_text& render(SDL_Surface*surf, const char* s, int x, int y, int fade=0, unsigned int color = 0xffffffff);
	_text& render(SDL_Surface*surf, const string& s, int x, int y, int fade=0, unsigned int color = 0xffffffff) {return render(surf, s.c_str(), x, y, fade, color);}
	int getW() {return width;}
	int getH() {return height;}
	void getSize(const char* s, int & w, int & h) {w = strlen(s) * width; h = height;}
};

extern _text text;

#endif //TEXT_H
