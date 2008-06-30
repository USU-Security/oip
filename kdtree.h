
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
	void collect(kdnode* r,float distance, float x, float y, vector<particle*>& near, float x0, float y0, float x1, float y1, bool plane);

	void erase(kdnode* r);
public:
	kdtree(vector<particle*> & plist);
	~kdtree();

	void collect(float distance, float x, float y, vector<particle*>& near);
};
	




#endif //QUADTREE_H
