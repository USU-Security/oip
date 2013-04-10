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

#include <string>
#include <sstream>
#include <iomanip>
#include <stdio.h>
using namespace std;
void hexprint(const unsigned char * b, int size)
{
	char tmp[10];
	while (size--)
	{
		snprintf(tmp, 10, "%02x ", (int)*b);
		b++;
	}
}
string hexit(unsigned char* b, int size)
{
	stringstream oss;
	int i;
	for (i = 0; i < size; i++)
		oss << hex << setw(2) << setfill('0') << (int)b[i] ;
	return oss.str();
}
