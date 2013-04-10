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
#ifndef IMAGE_H
#define IMAGE_H

#include<map>
#include<string>
#include<SDL/SDL.h>
using std::string;
using std::map;

class _image
{
private:
	map<string, SDL_Surface*> images;
public:
	~_image();

	SDL_Surface* operator()(const char* imgname);
};

extern _image image;


#endif //IMAGE_H
