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
#include <string>
using namespace std;

#include<SDL/SDL.h>


class entityset
{
private:
	map<int, entity> elist; //a map of entities
	float k; //the k constant for hookes law
	float dist; //the "right" distance
	float damp; //dampening factor
	float xscale, yscale; //scale between world and screen coords
	unsigned int net, mask;
public:
	entityset():k(.003),dist(.3),damp(.9) {net = config.ipvalue("localnet"); mask = config.ipvalue("localmask"); }
	entity& add(int s);
	void process(double dt);
	void draw(SDL_Surface*s);
	int size() const {return elist.size();}	
	entity* find(int x, int y);

	entity& operator[](int s) {return add(s);}
};

#endif //ENTITYSET_H

