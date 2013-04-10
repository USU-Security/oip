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

#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>
#include <vector>
#include <SDL/SDL.h>
using std::string;
using std::vector;

class particle
{
private:
	int size;
	float x, y, dx, dy;
	float speed; //what we need to cap our velocity at
	float damp; //keep particles from flying off with reckless abandon
	double created;
	unsigned int color;
	void move();
	bool softdelete;
	bool invalid;
public:
	unsigned int dst;

	particle() {invalid = true;}
	particle(float nx, float ny, float nsize, unsigned int ncolor, unsigned int s);

	//figures out how to move given particles and a destination
	void move(float dstx, float dsty, vector<particle*> & pnear, double dt);
	
	float getX() {return x;}
	float getY() {return y;}
	int getsize() { return size;}
	void draw(SDL_Surface* s);

	void erase() {softdelete = true;}
	bool deleteme() {return softdelete||invalid;}
};

#endif //PARTICLE_H
