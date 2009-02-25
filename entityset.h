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

#ifndef ENTITYSET_H
#define ENTITYSET_H

#include "entity.h"
#include "myconfig.h"
#include <map>
#include <vector>
using std::vector;
#include <string>
using namespace std;

#include<SDL/SDL.h>


class entityset
{
private:
	struct colornet {
		unsigned int ip, mask, color;
		colornet(unsigned int i, unsigned int m, unsigned int c)
		:ip(i), mask(m), color(c) {}
	};
	vector<colornet> colors;
	map<int, entity> elist; //a map of entities
	float k; //the k constant for hookes law
	float dist; //the "right" distance
	float damp; //dampening factor
	float xscale, yscale; //scale between world and screen coords
	void loadcolors();
public:
	entityset():k(.003),dist(.3),damp(.9) {loadcolors(); }
	entity& add(int s);
	void process(double dt);
	void draw(SDL_Surface*s);
	int size() const {return elist.size();}	
	entity* find(int x, int y);

	entity& operator[](int s) {return add(s);}
};

#endif //ENTITYSET_H

