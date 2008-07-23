
#ifndef TEXT_H
#define TEXT_H

#include <SDL/SDL.h>

#include <string.h>

#include <string>
using std::string;

//This class expects to be initialized with a single image, which
//contains ascii characters 32-126, the height and width of each character
class _text 
{
private:
	SDL_Surface*txt;
	int height;
	int width;
public:
	_text(const char* font, int w, int h);
	~_text() {if (txt) SDL_FreeSurface(txt);}
	_text& render(SDL_Surface*surf, const char* s, int x, int y, int fade=0);
	_text& render(SDL_Surface*surf, const string& s, int x, int y, int fade=0) {return render(surf, s.c_str(), x, y);}
	int getW() {return width;}
	int getH() {return height;}
	int getSize(const char* s, int & w, int & h) {w = strlen(s) * width; h = height;}
};

extern _text text;

#endif //TEXT_H
