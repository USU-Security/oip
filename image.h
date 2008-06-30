#ifndef IMAGE_H
#define IMAGE_H

#include<map>
#include<string>
#include<SDL/SDL.h>
using std::string;
using std::map;

class _image
{
private:
	map<string, SDL_Surface*> images;
public:
	~_image();

	SDL_Surface* operator()(const char* imgname);
};

extern _image image;


#endif //IMAGE_H
