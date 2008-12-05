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

#include "kdtree.h"

//see if point and other->point are within distance of each other
bool kdnode::closeto(float x, float y, float distance)
{
	if (point &&
		(x - point->getX()) *
		(x - point->getX()) +
		(y - point->getY()) *
		(y - point->getY()) < distance * distance )
		return true;
	return false;
}

/*
 * This function will divide the given list of nodes into two sets, based on which dimension we are dividing
 * precondition:
 * 		r is a node with no valid information
 * 		plist is a list of particles that need to be placed into r
 * postcondition:
 * 		r->point contains a particle from plist
 * 		r->left contains a new node  if there are particles less than r->point. likewies for r->right
 */
void kdtree::maketree(kdnode* r, vector<particle*> & plist, bool xplane)
{
	//select three random nodes
	particle* r1 = plist[rand() % plist.size()];
	particle* r2 = plist[rand() % plist.size()];
	particle* r3 = plist[rand() % plist.size()];
	particle* mid;
	vector<particle*> leftl;
	vector<particle*> rightl;
	typedef vector<particle*> myv;	//working around _yet_ another mvc++ bug
	if (xplane) //do we sort between the x coordinates?
	{
		if (r1->getX() < r2->getX() && r2->getX() < r3->getX())
			mid = r2;
		else if (r2->getX() < r1->getX() && r1->getX() < r3->getX())
			mid = r1;
		else
			mid = r3;
		//set the particle to the node, and divide the remainder
		
		myv::iterator i = plist.begin();
		//vector<particle*>::iterator i = plist.begin();
		for(;i!= plist.end(); i++)
		{
			if ((*i)->getX() < mid->getX())
				leftl.push_back(*i);
			else if ((*i)->getX() > mid->getX())
				rightl.push_back(*i);
		}
	}
	else
	{
		if (r1->getY() < r2->getY() && r2->getY() < r3->getY())
			mid = r2;
		else if (r2->getY() < r1->getY() && r1->getY() < r3->getY())
			mid = r1;
		else
			mid = r3;
		//set the particle to the node, and divide the remainder
		myv::iterator i = plist.begin();
		for(;i!= plist.end(); i++)
		{
			if ((*i)->getY() < mid->getY())
				leftl.push_back(*i);
			else if ((*i)->getY() > mid->getY())
				rightl.push_back(*i);
		}
	}
	//allocate two nodes, if necessary
	if (leftl.size())
	{
		r->left = new kdnode();
		maketree(r->left, leftl, !xplane);
	}
	if (rightl.size())
	{
		r->right = new kdnode();
		maketree(r->right, rightl, !xplane);
	}
	//save the particle at the midpoint
	r->point = mid;
}


/*
 * recursive function that gathers particles near a particular point. 
 * it walks through the tree, eliminating branches it doesnt need to search
 */
void kdtree::collect(kdnode* r, float distance, float x, float y, vector<particle*>& pnear, float x0, float y0, float x1, float y1, bool xplane)
{
	//is this nodes point within the distance?
	if (r->closeto(x, y, distance))
		pnear.push_back(r->point);
	if (xplane)
	{
		//what children should we consider?
		if (r->left && x > x0-distance && x < r->point->getX()+distance && 
			y > y0-distance && y < y1+distance)
			collect(r->left, distance, x, y, pnear, x0, y0, r->point->getX(), y1, false); 
		if (r->right && x > r->point->getX() -distance && x < x1 + distance && 
			y > y0-distance && y < y1+distance)
			collect(r->right, distance, x, y, pnear, r->point->getX(), y0, x1, y1, false);
	}
	else
	{
		//what children should we consider?
		if (r->left && x > x0-distance && x < y1+distance && 
			y > y0-distance && y < r->point->getY()+distance)
			collect(r->left, distance, x, y, pnear, x0, y0, x1, r->point->getY(), true); 
		if (r->right && x > x0-distance && x < x1 + distance && 
			y > r->point->getY()-distance && y < y1+distance)
			collect(r->right, distance, x, y, pnear, x0, r->point->getY(), x1, y1, true);
	}	
}

kdtree::kdtree(vector<particle*> & plist)
{
	if (plist.size())
	{
		root = new kdnode();
		maketree(root, plist, true);
	}
	else
		root = NULL;
}

void kdtree::erase(kdnode* r)
{
	if (r->left)
		erase(r->left);
	if (r->right)
		erase(r->right);
	r->left = NULL;
	r->right = NULL;
	delete r;
}

kdtree::~kdtree()
{
	if (root)
		erase(root);
	root = NULL;
}

void kdtree::collect(float distance, float x, float y, vector<particle*>&pnear)
{
	if (root)
		collect(root, distance, x, y, pnear, 0, 0, 1, 1, true);
}
