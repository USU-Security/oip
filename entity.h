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

#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <SDL/SDL.h>
using std::string;
#include "text.h"
#include "namecache.h"
/*
 * Defined to be a source/sink for the visualization. 
 * knows its position, label and brightness
 * Its position is between 0,0-1,1. it can be scaled to fit to whatever easily
 */
class entity
{
public:
	unsigned int label;
private:
	float brightness;
	float x;
	float y;
	float ndx, ndy;
	bool valid;
	bool fade;
	double lastupdate;
	float faderate;
	void init(unsigned int label);
public:
	entity(unsigned int l);
	entity() {init(0);valid=false;}

	void move(float dx, float dy, float damp, double dt);
	//TODO getX and getY are in world coordinates, yet getW and getH are not
	float getX() {return x;}
	float getY() {return y;}
	float getW() {return text.getW() * names[label].length();}
	float getH() {return text.getH();}

	void jump(float nx, float ny);

	bool draw(SDL_Surface*s);
	bool isvalid() {return valid;}

	bool deleteme() {return fade && fadeval>=15;}
	int getfadeval() {return fadeval;}
	void touch() {fade=false; }
	//two toggles that can publicly modified
	int fadeval;
	bool moving;
	bool resolve;
	void erase() {fade = true; fadeval = 15;}
	//these change the local fade. it is a multiplier between 1 and 4
	void faderatedecrease() { if (faderate < 4) faderate *= 1.01; }
	void faderateincrease() { if (faderate > 1) faderate *= .99;}
	//this sets the global fade rate based on the number of objects
	static void faderateset(int count);
	//the color that it draws as
	unsigned int color;
};

#endif //ENTITY_H
