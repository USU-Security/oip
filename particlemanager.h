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
	namecache names;
public:
	particlemanager():mouseisdown(NULL) {  }
	bool addpacket(unsigned int src, unsigned int  dst, unsigned int size, unsigned int color);
	~particlemanager();

	void process(double dt);
	void draw(SDL_Surface * s);

	void mousedown(int x, int y);
	void mousemove(int x, int y);
	void mouseup();
};

#endif //PARTICLEMANAGER_H
