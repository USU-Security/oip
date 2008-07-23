#include "font.h"
#include <iostream>
using std::cout;


namespace gui
{
	FT_Library library;

	void font::ref(bool addref)
	{
		static int refcount = 0;
		if (addref)
		{
			if (!refcount)
			{
				if (error = FT_Init_FreeType(&library))
					cout << "Unable to initialize the freetype library\n";
				else
					refcount++;
			}
		}
		else
		{
			refcount--;
			if (!refcount)
				FT_Done_FreeType(library);
		}
	}
			
	font::font(const char* path, int size)
	{
		//make sure the library is initialized
		error = 0;
		ref(true);
		if (error = FT_New_Face(library, path, 0, &face))
			cout << "Unable to load font " << path << "\n";
		else
			setSize(size);
	}
	font::~font()
	{
		ref(false);
	}

	void font::setSize(int size)
	{
		fontsize = size;
		if (error = FT_Set_Char_Size(face, 0, fontsize*64, 0, 0))
			cout << "Unable to set the size of the font\n";
	}

	void font::render(const string& s, int x, int y, SDL_Surface* surf)
	{
		SDL_LockSurface(surf);
		string::const_iterator i;
		for (i = s.begin(); i != s.end(); i++)
		{
			int glyph_index = FT_Get_Char_Index(face, *i);
			if (error = FT_Load_Char(face, *i, FT_LOAD_RENDER))
				cout << "Unable to load the glyph for character '" << *i << "'\n";
			else
			{
				draw_glyph(x+face->glyph->bitmap_left, 
						   y-face->glyph->bitmap_top, 
						   &face->glyph->bitmap, surf);	
				x += face->glyph->advance.x >> 6;
				y += face->glyph->advance.y >> 6;
			}
		}
		SDL_UnlockSurface(surf);
	}

	void font::draw_glyph(int x, int y, FT_Bitmap* bmp, SDL_Surface* s)
	{
		int i, j;
		//crop it
		int minx = x < 0 ? -x : 0;
		int maxx = x + bmp->width > s->w ? s->w - x : bmp->width;
		int miny = y < 0 ? -y : 0;
		int maxy = y + bmp->rows > s->h ? s->h - y : bmp->rows;
		Uint8 *p = (Uint8*)s->pixels + (y+miny) * s->pitch + (x+minx) * s->format->BytesPerPixel;
		unsigned char* data = bmp->buffer + miny*bmp->pitch + minx;
		for (j = miny; j < maxy; j++)
		{
			for (i = 0; i < maxx-minx; i++)
			{
				*((Uint32*)p+i) = SDL_MapRGBA(s->format, *data,*data,*data, *data);
				data++;
			}
			data += bmp->pitch;
			data -= maxx-minx;
			p += s->pitch;
		}
	}
};
		
