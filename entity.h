
#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <SDL/SDL.h>
using std::string;
#include "text.h"

/*
 * Defined to be a source/sink for the visualization. 
 * knows its position, label and brightness
 * Its position is between 0,0-1,1. it can be scaled to fit to whatever easily
 */
class entity
{
public:
	string label;
private:
	float brightness;
	float x;
	float y;
	float ndx, ndy;
	bool valid;
	bool fade;
	int fadeval;
	double lastupdate;
	void init(const string& label);
public:
	entity(const string& label);
	entity(const char* label);
	entity() {init(string(""));valid=false;}

	void move(float dx, float dy, float damp, double dt);
	//TODO getX and getY are in world coordinates, yet getW and getH are not
	float getX() {return x;}
	float getY() {return y;}
	float getW() {return text.getW() * label.length();}
	float getH() {return text.getH();}

	void jump(float nx, float ny);

	bool draw(SDL_Surface*s);
	bool isvalid() {return valid;}

	bool deleteme() {return fade && fadeval>=15;}
	int getfadeval() {return fadeval;}
	void touch() {fade=false;}
	bool moving;
};

#endif //ENTITY_H
