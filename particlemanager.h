/*
 * This file handles particles and entities, and the interactions between them. 
 */
#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include "particle.h"
#include "entityset.h"

class particlemanager
{
private:
	entityset entities;
	vector<particle*> particles;
	float xscale, yscale;
	int mx, my;
	entity* mouseisdown;
public:
	particlemanager():mouseisdown(NULL) {  }
	void packet(const string& src, const string& dst, float size, unsigned int color);
	~particlemanager();

	void process(double dt);
	void draw(SDL_Surface * s);

	void mousedown(int x, int y);
	void mousemove(int x, int y);
	void mouseup();
};

#endif //PARTICLEMANAGER_H
