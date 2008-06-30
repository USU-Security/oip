#include "image.h"
#include "text.h"
#include <SDL/SDL_image.h>

_image::~_image()
{
	map<string, SDL_Surface*>::iterator i = images.begin();
	for(;i != images.end(); i++)
		SDL_FreeSurface(i->second);
}

SDL_Surface* _image::operator()(const char* imgname)
{
	map<string, SDL_Surface*>::iterator i = images.find(string(imgname));
	if (i != images.end())
		return i->second;
	SDL_Surface* n = IMG_Load(imgname);	
	SDL_SetAlpha(n, SDL_SRCALPHA, 255);
	images[string(imgname)] = n;
	return n;
}

_image image;
