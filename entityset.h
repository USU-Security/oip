
#ifndef ENTITYSET_H
#define ENTITYSET_H

#include "entity.h"
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
public:
	entityset():k(.003),dist(.3),damp(.9) {}
	entity& add(int s);
	void process(double dt);
	void draw(SDL_Surface*s);
	
	entity* find(int x, int y);

	entity& operator[](int s) {return add(s);}
};

#endif //ENTITYSET_H

