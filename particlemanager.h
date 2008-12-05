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
/*
 * This file handles particles and entities, and the interactions between them. 
 */
#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include "particle.h"
#include "entityset.h"
#include "packetsink.h"
#include "namecache.h"

class particlemanager: public packetsink
{
private:
	entityset entities;
	vector<particle*> particles;
	float xscale, yscale;
	int mx, my;
	entity* mouseisdown;
	bool oldmoving;
public:
	particlemanager():mouseisdown(NULL) {  }
	bool addpacket(unsigned int src, unsigned int  dst, unsigned int size, unsigned int color);
	~particlemanager();

	void process(double dt);
	void draw(SDL_Surface * s);

	void mousedown(int x, int y);
	void mousemove(int x, int y);
	entity* bycoords(int x, int y) { return entities.find(x, y);}
	void mouseup();
};

#endif //PARTICLEMANAGER_H
