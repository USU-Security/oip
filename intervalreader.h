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

