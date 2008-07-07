
#include <string>
#include <sstream>
#include <iomanip>
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
