/*
 * Connects to a given oip server. displays the resulting stream. 
 */


#include <iostream>
#include <iomanip>
#include <SDL/SDL.h>
#include <stdlib.h>
#include "text.h"
#include "particlemanager.h"
#include <signal.h>
#include <sstream>
#include "clientpm.h"

using namespace std;

#define OPTSIZE 3
const char* validopts[OPTSIZE][4] = {
	/*short, long, description, required*/
	{"s", "server", "The hostname or ip address of the server.",""},
	{"p", "port", "The port to connect to. Defaults to 12751.",NULL},
	{"f", "filter", "The libpcap filter to apply to the stream.",NULL}
};

const char* optlookup(const char * o)
{
	int i;
	for (i = 0; i < OPTSIZE; i++)
	{
		if (!strcmp(o, validopts[i][0]) || !strcmp(o, validopts[i][1]))
		{
			validopts[i][3] = NULL; //mark it as found
			return validopts[i][1];
		}
	}
	return NULL;
}

bool optsvalid()
{
	int i;
	bool ret = true;
	for (i = 0; i < OPTSIZE; i++)
	{
		if (validopts[i][3])
		{
			ret = false;
			cout << "Missing required argument: --" << validopts[i][1] << endl;
		}
	}
	return ret;
}

bool parseopts(int argc, char** argv, map<string, string>& opts)
{
	int i=1;
	opts["port"] = "12751";
	while (i < argc)
	{
		const char *curopt, *val;
		char shortopt[2] = {0,0};
		if (i+1 < argc && strlen(argv[i]) > 3 && argv[i][0] == '-' && argv[i][1] == '-')
		{
			curopt = argv[i++] + 2;
			val = argv[i++];
		}
		else if (i+1 < argc && strlen(argv[i]) == 2 && argv[i][0] == '-')
		{
			shortopt[0] =  argv[i++][1];
			curopt = shortopt;
			val = argv[i++];
		}
		else if (strlen(argv[i]) > 2 && argv[i][0] == '-')
		{
			shortopt[0] = argv[i][1];
			curopt = shortopt;
			val = argv[i++] + 2;
		}
		else 
		{
			cout << "Invalid option syntax. \n";
			return false;
		}
		const char *valid;
		if (valid = optlookup(curopt))
			opts[valid] = val;
		else
		{
			cout << "Unknown option: " << curopt << "\n";
			return false;
		}
	}
	return optsvalid();
}

void usage(const char* appname)
{
	int i;
	cout << "Usage: appname [options]\n";
	for (i = 0; i < OPTSIZE; i++)
	{
		cout << "  -" << validopts[i][0] 
			 << " --" << left << setw(10) << validopts[i][1]
			 << " "  << validopts[i][2];
		if (validopts[i][3])
			cout << " Required.\n";
		else
			cout << " Optional.\n";
	}
}



inline double now()
{
	//struct timeval tv;
	//gettimeofday(&tv, NULL);
	//return tv.tv_sec + tv.tv_usec/1000000.0;
	return SDL_GetTicks()/1000.0;
}

SDL_Surface* resize(int w, int h)
{
	SDL_Surface * screen;
	if (!(screen = SDL_SetVideoMode(w,h,32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_RESIZABLE)))
	{
		cout << "Unable to get a hardware surface, falling back on software\n";
		screen = SDL_SetVideoMode(w,h,32, SDL_SWSURFACE|SDL_RESIZABLE);
	}
	if (!screen) 
	{
		cout << "Unable to set display: " << SDL_GetError() << "\n";
		return NULL;
	}
	return screen;
}

SDL_Surface* initsdl()
{
	if ( SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "Unable to initialize sdl: " << SDL_GetError() << "\n";
		return NULL;
	}
	atexit(SDL_Quit);
	return resize(512,512);
}

void sigintcatch(int s)
{
	exit(0);
}

#ifdef WIN32
#define MAXARGS 512
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmdline, int nShowCmd)
{
	int argc=1;
	char* argv[MAXARGS];
	char exename[_MAX_PATH];
	GetModuleFileName(NULL, exename, _MAX_PATH);
	argv[0] = exename;
	char* args = new char[strlen(cmdline)];
	argv[argc++] = args;
	while (*cmdline)
	{
		if (*cmdline == '\\' && *(cmdline + 1))
		{
			cmdline++;
			switch(*cmdline)
			{
			case 'n':
				*args = '\n';
				break;
			case 't':
				*args = '\t';
				break;
			case 'r':
				*args = '\r';
				break;
			default:
				*args = *cmdline;
				break;
			}
			args++; cmdline++;
		}
		else if (*cmdline == ' ')
		{
			*args = 0;
			while (*cmdline && *cmdline == ' ')
				cmdline++;
			args++;
			argv[argc++] = args;
		}
		else
		{
			*args = *cmdline;
			args++; cmdline++;
		}
	}
	*args=0; //just in case;
	//winsock startup code
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	int ret = main(argc, argv);
	WSACleanup();
//	delete [] argv[1];
	return ret;
}
#endif

int main(int argc, char* argv[])
{
	bool run=true;
	map<string, string> opts;

	//try to die gracefully
	signal(SIGINT, sigintcatch);

	//parse the options
	if (!parseopts(argc, argv, opts))
	{
		usage(argv[0]);
		return 1;
	}
	Uint16 port = atoi(opts["port"].c_str());
	opts.erase("port");
	string server = opts["server"];
	opts.erase("server");
	particlemanager pm;

	//initialize the graphics
	SDL_Surface* screen = initsdl();
	if (!screen)
		return 1;

	//and lastly, try to connect to the server
	clientpm packetlist(server.c_str(), opts, port);

	double ti = now();
	while(run)
	{
	
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT: 
				run = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					run = false;
				break;
			case SDL_VIDEORESIZE:
				resize(event.resize.w, event.resize.h);
				break;
			case SDL_MOUSEBUTTONDOWN:
				pm.mousedown(event.button.x, event.button.y);
				break;
			case SDL_MOUSEMOTION:
				pm.mousemove(event.button.x, event.button.y);
				break;
			case SDL_MOUSEBUTTONUP:
				pm.mouseup();
				break;
				
			}
		}
		double dt = now() - ti;
		ti = now();
		packetlist.dumpdata(pm);
		pm.process(dt);
		SDL_FillRect(screen, NULL, 0);
		pm.draw(screen);
		stringstream ss;
		ss << 1.0/dt << " fps";
		text.render(screen, ss.str().c_str(), 0, 0, 8);
		SDL_Flip(screen);
		//don't bother going faster than twice the minimum framerate
		if (dt * 1000 < MINRATE)
			SDL_Delay(MINRATE - (int)(dt * 1000));
	}

	return 0;
}

