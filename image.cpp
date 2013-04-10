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
#include "image.h"
#include "text.h"
#include <SDL/SDL_image.h>

#include <iostream>
using namespace std;

_image::~_image()
{
	typedef map<string, SDL_Surface*> mapssurf; //vc++ workaround
	mapssurf::iterator i = images.begin();
	for(;i != images.end(); i++)
	{
		cout << "Freeing surface " << i->first << endl;
		SDL_FreeSurface(i->second);
	}
}

SDL_Surface* _image::operator()(const char* imgname)
{
	typedef map<string, SDL_Surface*> mapssurf; //vc++ workaround
	mapssurf::iterator i = images.find(string(imgname));
	if (i != images.end())
		return i->second;
	SDL_Surface* n = IMG_Load(imgname);	
	SDL_SetAlpha(n, SDL_SRCALPHA, 255);
	images[string(imgname)] = n;
	return n;
}

_image image;
