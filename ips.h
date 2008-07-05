
#ifndef IPS_H
#define IPS_H
inline char* longtoip(char* b, int s, unsigned int sip)
{
	snprintf(b, s, "%d.%d.%d.%d", sip >> 24, (sip >> 16)&255, (sip >> 8)&255, sip & 255);
	return b;
}

#endif //IPS_H
