#include "entityset.h"

entity& entityset::add(int s)
{
	entity& r = elist[s];
	if (!r.isvalid())
		return elist[s] = entity(s);
	r.touch();
	return r;
}

void entityset::process(double dt)
{
	//go through all of the entities, and apply hookes law to each of them
	map<int, entity>::iterator i = elist.begin();
	for(;i != elist.end(); i++)
	{
		float fx=0, fy=0;
		map<int, entity>::iterator j = elist.begin();
		//sum up the total force applied by all the others
		float rsq;
		for (;j != elist.end(); j++)
		{
			if ((*i).first != (*j).first)
			{
				//hookes law; linear attraction
				fx -= k*(((*i).second.getX() - (*j).second.getX()));
				fy -= k*(((*i).second.getY() - (*j).second.getY()));
				//coulomb repulsion
				rsq = 
					((*i).second.getX() - (*j).second.getX()) *
					((*i).second.getX() - (*j).second.getX()) +
					((*i).second.getY() - (*j).second.getY()) *
					((*i).second.getY() - (*j).second.getY());
				//some magic numbers. The "charge" of the particle goes away as it fades
				rsq /= k/(4); // * (1+(*j).second.getfadeval()));
				fx += ((*i).second.getX() - (*j).second.getX())/rsq;
				fy += ((*i).second.getY() - (*j).second.getY())/rsq;
			}
			//add a slight jitter so that they don't get stuck in the middle
			fx += (float)rand()/RAND_MAX * .001 - .0005;
			fy += (float)rand()/RAND_MAX * .001 - .0005;
			//rather than using hookes law, what if we just repulse from the sides?
			//they should distribute themselves neatly
			//They appear to just slap themselves on the sides
			/*
			//left side
			rsq = 
				((*i).second.getX()) *
				((*i).second.getX());
			rsq /= k/100;
			fx += ((*i).second.getX())/rsq;
			//right side
			rsq = 
				((*i).second.getX() - 1) *
				((*i).second.getX() - 1);
			rsq /= k/100;
			fx += ((*i).second.getX() - 1)/rsq;
			//top
			rsq = 
				((*i).second.getY()) *
				((*i).second.getY());
			rsq /= k/100;
			fy += ((*i).second.getY())/rsq;
			//bottom
			rsq = 
				((*i).second.getY() - 1) *
				((*i).second.getY() - 1);
			rsq /= k/100;
			fy += ((*i).second.getY() - 1)/rsq;
			// */
		}
		


		(*i).second.move(fx, fy, damp, dt);
	}
}

void entityset::draw(SDL_Surface*s)
{
	xscale = s->w;
	yscale = s->h;
	
	map<int, entity>::iterator j = elist.begin();
	//draw them, and do some housekeeping
	while (j != elist.end())
	{
		(*j).second.draw(s);
		if ((*j).second.deleteme())
			elist.erase(j++);
		else
			j++;
	}
}

/*
 * find an entity that contains the location, given in screen coords
 */
entity* entityset::find(int x, int y)
{
	map<int, entity>::iterator j = elist.begin();
	for (;j != elist.end();j++)
	{
		//TODO getX and getY are in world coordinates, yet getW and getH are not
		if (
			(*j).second.getX() * xscale - (*j).second.getW()/2 < x &&
			(*j).second.getY() * yscale - (*j).second.getH()/2 < y &&
			(*j).second.getX() * xscale + (*j).second.getW()/2 > x &&
			(*j).second.getY() * yscale + (*j).second.getH()/2 > y
			)
			return &((*j).second);
	}
	return NULL;
}

