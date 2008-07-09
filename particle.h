
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
	unsigned int color;
	void move();
	bool softdelete;
	bool invalid;
public:
	const string dst;

	particle() {invalid = true;}
	particle(float nx, float ny, float nsize, unsigned int ncolor, const string& s);

	//figures out how to move given particles and a destination
	void move(float dstx, float dsty, vector<particle*> & near, double dt);
	
	float getX() {return x;}
	float getY() {return y;}
	int getsize() { return size;}
	void draw(SDL_Surface* s);

	void erase() {softdelete = true;}
	bool deleteme() {return softdelete||invalid;}
};

#endif //PARTICLE_H
