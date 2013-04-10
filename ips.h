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

#ifndef IPS_H
#define IPS_H
#include <stdlib.h>
#ifndef WIN32
inline char* longtoip(char* b, int s, unsigned int sip)
{
	snprintf(b, s, "%d.%d.%d.%d", sip >> 24, (sip >> 16)&255, (sip >> 8)&255, sip & 255);
	return b;
}
#else
#include <strstream>
#include <iomanip>
using std::strstream;
inline char* longtoip(char* b, int s, unsigned int sip)
{
	strstream ss(b, s);
	ss << (sip >> 24);
	ss << ".";
	ss << ((sip >> 16)&255);
	ss << ".";
	ss << ((sip >> 8)&255);
	ss << ".";
	ss << (sip & 255);
	ss << '\0';
	return b;
}
#endif

inline unsigned int iptolong(const char* b)
{
	const char* a[4] = {0};
	int i=0;
	a[i++] = b;
	while (*b)
	{
		if (*b == '.')
		{
			b++;
			if (b)
				a[i++] = b;
			if (i == 4)
				break;
		}
		b++;
	}
	if (i == 4)
		return (atoi(a[3]) << 24) + (atoi(a[2]) << 16) + (atoi(a[1]) << 8) + atoi(a[0]);
	throw string("Unable to parse an ip address from the string '") + b + "'\n";
	return 0;
}

#endif //IPS_H
