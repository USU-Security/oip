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

#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
using std::vector;

#include "particle.h"

class kdnode
{
public:
	kdnode * left;
	kdnode * right;
	particle * point;

	kdnode():left(0),right(0),point(0) {}

	bool closeto(float x, float y, float distance);
};

class kdtree
{
private:
	kdnode * root;

	void maketree(kdnode* r, vector<particle*> & plist, bool xplane);
	void collect(kdnode* r,float distance, float x, float y, vector<particle*>& pnear, float x0, float y0, float x1, float y1, bool plane);

	void erase(kdnode* r);
public:
	kdtree(vector<particle*> & plist);
	~kdtree();

	void collect(float distance, float x, float y, vector<particle*>& pnear);
};
	




#endif //QUADTREE_H
