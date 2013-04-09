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

#include <fontconfig/fontconfig.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "font.h"

#include <iostream>
using std::cout;


namespace gui
{
	FcConfig *config;
	FT_Library freetype;

	void font::ref(bool addref)
	{
		static int refcount = 0;
		if (addref)
		{
			if (!refcount)
			{
				if(!FcInit()) {
					fprintf(stderr, "Error initializing fontconfig.\n");
					exit(1);
				}



				if((error = FT_Init_FreeType(&freetype))) {
					fprintf(stderr, "Error initializing Freetype2: %d\n", error);
					exit(1);
				}
			}
			else
			{
				refcount++;
			}
		}
		else
		{
			refcount--;
			if (!refcount)
				FT_Done_FreeType(freetype);
		}
	}
			
	font::font(unsigned char* font_name, double font_size)
	{
		FcPattern *pattern, *fpat;
		FcResult result = FcResultMatch;
		FcChar8 *filename;
		int font_index;
		FcMatrix *font_matrix = NULL;

		if(font_name == NULL)
		{
			font_name = (unsigned char *)strdup(DEFAULT_FONT_NAME);
		}

		//make sure the library is initialized
		error = 0;
		ref(true);

		if((pattern = FcNameParse((FcChar8*)font_name)) == NULL) {
			fprintf(stderr, "Error parsing font name.\n");
			exit(1);
		}
		if(!FcConfigSubstitute(NULL, pattern, FcMatchPattern)) {
			fprintf(stderr, "Error substituting configuration.\n");
			exit(1);
		}
		FcDefaultSubstitute(pattern);
		if((fpat = FcFontMatch(NULL, pattern, &result)) == NULL ||
				result != FcResultMatch) {
			fprintf(stderr, "Error matching the font.\n");
			exit(1);
		}
		if(FcPatternGetString(fpat, FC_FILE, 0, &filename) != FcResultMatch) {
			fprintf(stderr, "Error finding the font name.\n");
			exit(1);
		}
		if(FcPatternGetInteger(fpat, FC_INDEX, 0, &font_index) != FcResultMatch) {
			fprintf(stderr, "Error finding the font index.\n");
			exit(1);
		}
		if(FcPatternGetDouble(fpat, FC_SIZE, 0, &font_size) != FcResultMatch) {
			fprintf(stderr, "Error finding the font size.\n");
			exit(1);
		}
		if(FcPatternGetMatrix(fpat, FC_MATRIX, 0, &font_matrix) != FcResultMatch) {
			font_matrix = NULL; /* make sure */
		}

		if (error = FT_New_Face(freetype, (char *)filename, font_index, &face))
		{
			cout << "Unable to load font " << filename << "\n";
		}
		else
		{
			setSize(font_size);
		}

		if(font_matrix != NULL) {
			FT_Matrix m;
			FT_Vector v;
			m.xx = font_matrix->xx * 65536;
			m.xy = font_matrix->xy * 65536;
			m.yx = font_matrix->yx * 65536;
			m.yy = font_matrix->yy * 65536;
			v.x = v.y = 0;
			FT_Set_Transform(face, &m, &v);
		}

		color = 0xffffffff; //set to white
	}
	font::~font()
	{
		ref(false);
	}

	void font::setSize(double size)
	{

		fontsize = size;
		if (error = FT_Set_Char_Size(face, 0, fontsize*64, 0, 0))
		{
			cout << "Unable to set the size of the font\n";
		}
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
		h = fontsize * 64;
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
		
