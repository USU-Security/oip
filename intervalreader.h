#ifndef INTERVALREADER_H
#define INTERVALREADER_H

#include "packetmanager.h"

class intervalreader: public packetmanager
{
protected:
	double start;
	double current;
	double stop;
	double rate;
	double ti;
public:
	intervalreader():current(0),stop(1),rate(1),ti(SDL_GetTicks()/1000.0),start(0) {}

	virtual double getcurrent() { return current/(stop-start); }
	virtual void setcurrent(double c) {if (c < 0) current = 0; else if (c > 1) current = stop-start; else current = c * (stop-start);}

	double getrate() {return rate;}
	void increase() {rate *= 2;}
	void decrease() {rate *= .5;}
};

#endif//INTERVALREADER_H

