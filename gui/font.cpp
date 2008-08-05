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
		color = 0xffffffff; //set to white
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
				Uint8 r1,g1,b1,a1,r2,g2,b2;
				SDL_GetRGBA(*((Uint32*)p+i), s->format, &r1, &g1, &b1, &a1);
				SDL_GetRGB(color, s->format, &r2, &g2, &b2);
				if (*data)
				{
					float a2 = *data/255.0;
					//use an "over" compositing function
					*((Uint32*)p+i) = SDL_MapRGBA(s->format, 
						(int)(r1*(1-a2) + r2*a2),
						(int)(g1*(1-a2) + g2*a2),
						(int)(b1*(1-a2) + b2*a2),
						(int)(a1*(1-a2) + *data*a2));
				}
				data++;
			}
			data += bmp->pitch;
			data -= maxx-minx;
			p += s->pitch;
		}
	}

	void font::getSize(const string& s, int& w, int& h)
	{
		string::const_iterator i;
		w = 0;
		h = fontsize << 6;
		int maxdrop = 0;
		for (i = s.begin(); i != s.end(); i++)
		{
			int glyph_index = FT_Get_Char_Index(face, *i);
			if (error = FT_Load_Char(face, *i, FT_LOAD_DEFAULT))
				cout << "Unable to load the glyph for character '" << *i << "'\n";
			else
			{
				w += face->glyph->advance.x;
				h += face->glyph->advance.y;
				if (maxdrop < face->glyph->metrics.height - face->glyph->metrics.horiBearingY)
					maxdrop = face->glyph->metrics.height - face->glyph->metrics.horiBearingY;
			}
		}
		h += maxdrop;
		h >>= 6;
		w >>= 6;
	}


};
		
