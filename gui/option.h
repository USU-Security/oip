#ifndef GUI_OPTION_H
#define GUI_OPTION_H

#include "button.h"

namespace gui
{
	class option: public button
	{
	protected:
		bool state;
	public:
		option(const char* upimg, const char* downimg, clickevent c, int o=5):button(upimg, downimg, c, o),state(false) {}


		virtual bool keyup(SDL_KeyboardEvent& k);
		virtual bool keydown(SDL_KeyboardEvent&k);
	};
};


#endif//GUI_LABEL_H
