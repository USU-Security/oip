
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
	return b;
}
#endif

#endif //IPS_H
