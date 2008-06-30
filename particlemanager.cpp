#include "particlemanager.h"
#include "kdtree.h"

#include <iostream>
using std::cout;

/*
 * creates a packet starting at src, heading toward dst, with a specific size
 */
void particlemanager::packet(const string& src, const string& dst, float size, unsigned int color)
{
	entity& e1 = entities[src];
	particles.push_back(new particle(e1.getX(), e1.getY(), size, color, dst));
}

particlemanager::~particlemanager()
{
	vector<particle*>::iterator i;
	for (i = particles.begin(); i != particles.end(); i++)
		delete *i;
}

void particlemanager::process(double dt)
{
	vector<particle*>::iterator i;
	//kreate a kd tree with all the particles in it
	//*
	kdtree pfind(particles);
	for (i = particles.begin(); i != particles.end(); i++)
	{
		entity& e1 = entities[(*i)->dst];
		//check if it is at its destination
		if ((e1.getX() - (*i)->getX())*(e1.getX() - (*i)->getX()) +
			(e1.getY() - (*i)->getY())*(e1.getY() - (*i)->getY()) < .0005)
			(*i)->erase();
		vector<particle*> pnear;
		pfind.collect(.2, (*i)->getX(), (*i)->getY(), pnear);

		(*i)->move(e1.getX(), e1.getY(), pnear, dt);
	}
	// */
	entities.process(dt);
}

void particlemanager::draw(SDL_Surface* s)
{
	xscale = s->w;
	yscale = s->h;
	vector<particle*>::iterator i = particles.begin();
	//draw them, and delete them if they are marked for deletion
	while (i != particles.end())
	{
		(*i)->draw(s);
		if ((*i)->deleteme())
		{
			particle* d = *i;
			*i = particles.back();
			particles.pop_back();
			delete d;
		}
		else
			i++;
	}
	entities.draw(s);
}

//search for the entity they clicked on
void particlemanager::mousedown(int x, int y)
{
	mouseisdown = entities.find(x, y);
	if (mouseisdown)
	{
		mx = (int)(mouseisdown->getX() * xscale - x);
		my = (int)(mouseisdown->getY() * yscale - y);
		mouseisdown->moving = false;
	}
}

//move the given entity if we need to. 
void particlemanager::mousemove(int x, int y)
{
	if (mouseisdown)
		mouseisdown->jump((x + mx)/xscale, (y+ my)/yscale);
}

//don't move the stuff anymore
void particlemanager::mouseup()
{
	if (mouseisdown)
	{
		mouseisdown->moving = true;
		mouseisdown = NULL;
	}
}
