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

#endif //IPS_H
