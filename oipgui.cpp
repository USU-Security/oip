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
#include "gui/option.h"
#include "gui/layout.h"
#include "gui/textbox.h"
#include "chart.h"
#include "capreader.h"
#include "ips.h"
#ifndef WIN32
#include <stdlib.h>
#include <unistd.h>
#endif
using namespace std;

#define OPTSIZE 5
const char* validopts[OPTSIZE][4] = {
	/*short, long, description, required*/
	{"s", "server", "The hostname or ip address of the server.",NULL},
	{"p", "port", "The port to connect to. Defaults to 12751.",NULL},
	{"f", "filter", "The libpcap filter to apply to the stream.",NULL},
	{"e", "speed", "How fast to read the pcap file. 1000 is normal, 100 is 10x, 10 is 100x, 1 is 1000x",NULL},
	{"c", "pcap", "Pcap file to read",NULL}
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
	if (argv[1] == args) //this will be true if there were no arguments
		argc--;
	*args=0; //just in case;
	//winsock startup code
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	int ret = main(argc, argv);
	WSACleanup();
	//delete [] argv[1];
	return ret;
}
#endif

void mnutoggle(bool selected, void* arg)
{
	gui::layout* self = (gui::layout*)arg;
	if (selected)
		self->show(gui::TOP);
	else
		self->hide(gui::TOP);
}
struct pcapopts
{
	packetmanager** plist;
	gui::textbox* pcapfile;
	gui::option* mnu;
	pcapopts(packetmanager*&pl, gui::textbox*p, gui::option*m)
	:plist(&pl), pcapfile(p), mnu(m) {}
};
void newpcapfile(bool selected, void* arg)
{
	pcapopts* self = (pcapopts*)arg;
	if (*(self->plist))
		delete *(self->plist);

	*(self->plist) = new capreader(self->pcapfile->getString().c_str());

	SDL_WM_SetCaption((string("PCAP: ") + self->pcapfile->getString()).c_str(), "Oip");
	self->mnu->activate();

	cout << "OIPGUI.CPP; newcapfile(): PCAP file running!\n";
}

struct servopts
{
	packetmanager** plist;
	gui::textbox* server;
	gui::textbox* port;
	gui::textbox* filter;
	gui::textbox* speed;
	gui::textbox* pcap_file_to_read;
	gui::option* mnu;
	servopts(packetmanager*&pl, gui::textbox* s, gui::textbox*p, gui::textbox*f, gui::option* m)
	:plist(&pl),server(s),port(p),filter(f),mnu(m) {}
};
void newconnection(bool selected, void* arg)
{
	servopts *self = (servopts*)arg;
	if (*(self->plist))
		delete *(self->plist);
	map<string, string> opts;
	string f = self->filter->getString();
	if (f != "")
		opts["filter"] = f;
	*(self->plist) = new clientpm(
		self->server->getString().c_str(), 
		opts, 
		atoi(self->port->getString().c_str())
		);
	SDL_WM_SetCaption((self->server->getString() + ":" + f).c_str(), "Oip");
	self->mnu->activate();
	
}
void killconnection(bool selected, void* arg)
{
	packetmanager** pm = (packetmanager**)arg;
	if (*pm)
	{
		delete *pm;
		*pm = 0;
	}
}

void togglebool(bool selected, void* arg)
{
	bool* b = (bool*)arg;
	if (b)
	{
		*b = !*b;
		cout << "option set to " << *b << "\n";
	}
}

struct cxargs
{
	const char* s;
	unsigned int ip;
};

inline unsigned int swaplong(unsigned int i)
{
	return (i >> 24) | ((i >> 8)&0xff00) | ((i << 8)&0xff0000) | (i << 24);
}

void customexecute(bool selected, void* arg)
{
	cxargs* args = (cxargs*)arg;
	char ip[32];
	char buffer[512];
	snprintf(buffer, 512, args->s, longtoip(ip, 32, swaplong(args->ip)));
	cout << "im supposed to do a system(" << buffer << ")\n";
	if (0 == fork())
	{
		system(buffer);
		_exit(0);
	}
}

int main(int argc, char* argv[])
{
	bool run=true;
	map<string, string> opts;
	packetmanager * packetlist = NULL; // packetlist(server.c_str(), opts, port);

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
	vector<string> customcommands, customlabels;
	config.values("customcmd", customcommands);
	int i;
	for (i = 0; i < customcommands.size(); i++)
	{
		int j;
		for (j = 0; j < customcommands[i].size(); j++)
		{
			if (customcommands[i][j] == ' ')
				break;
		}
		if (j < customcommands[i].size())
		{
			customlabels.push_back(customcommands[i].substr(0, j));
			customcommands[i] = customcommands[i].substr(j+1);
		}
		else
			customlabels.push_back("");
		cout << "custom command #" << i << " '" << customlabels[i] << "' => '" << customcommands[i] << "'\n";
	}
	


	// Pcap file passed by argument
	string pcap_file_to_read = opts["pcap"];
	opts.erase("pcap");

	string speed = opts["speed"];

	//initialize the graphics
	SDL_Surface* screen = initsdl();
	if (!screen)
		return 1;

	//init some widgets
	gui::font mnufont;
	mnufont.setSize(16);
	//the labels for the connection menu
	gui::label serverLabel(70,24);
	serverLabel.setFont(mnufont);
	serverLabel.setString("Server:");
	gui::label portLabel(50,24);
	portLabel.setFont(mnufont);
	portLabel.setString("Port:");
	gui::label filterLabel(50,24);
	filterLabel.setFont(mnufont);
	filterLabel.setString("Filter:");
	//the textboxes
	gui::textbox serverTxt(DATADIR "mnubg.png");
	serverTxt.setFont(mnufont);
	serverTxt.setString(server);
	gui::textbox portTxt(DATADIR "mnubg.png");
	portTxt.setFont(mnufont);
	portTxt.setString("12751");
	gui::textbox filterTxt(DATADIR "mnubg.png");
	filterTxt.setFont(mnufont);
	if (opts.find("filter") != opts.end())
		filterTxt.setString(opts["filter"]);
	//the buttons
	gui::layout menu;
	gui::option mnu(DATADIR "mnuup.png", DATADIR "mnudn.png", mnutoggle, &menu);
	servopts s(packetlist, &serverTxt, &portTxt, &filterTxt, &mnu);
	gui::button btnnewconn(DATADIR "mnubg.png",DATADIR "mnusel.png", newconnection, &s, 1);
	btnnewconn.setFont(mnufont);
	btnnewconn.setString("Connect");
	gui::button btndisconnect(DATADIR "mnubg.png",DATADIR "mnusel.png", killconnection, &packetlist, 1);
	btndisconnect.setFont(mnufont);
	btndisconnect.setString("Disconnect");
	//the layout
	gui::layout connectionmenu;
	connectionmenu.addchild(serverLabel, 0,1);
	connectionmenu.addchild(serverTxt, 70,1);
	connectionmenu.addchild(portLabel, 0, 26);
	connectionmenu.addchild(portTxt, 70, 26);
	connectionmenu.addchild(filterLabel, 0, 51);
	connectionmenu.addchild(filterTxt,70, 51);
	connectionmenu.addchild(btnnewconn, 35, 80);
	connectionmenu.addchild(btndisconnect, 35, 110);
	connectionmenu.hide();
	connectionmenu.hasborder = 1;
	gui::option connection(DATADIR "mnubg.png",DATADIR "mnusel.png",mnutoggle,&connectionmenu,1);
	connection.setFont(mnufont);
	connection.setString("Connection");
	menu.addchild(connection, 0, 0);
	menu.addchild(connectionmenu, 0, 30);
	
	//the capreader menu
	gui::textbox pcapfile(DATADIR "mnubg.png");
	pcapfile.setFont(mnufont);
	pcapfile.setString("/home/wojtyla/VirtualEnviroment/Captures/test.pcap");
	if (pcap_file_to_read != "") {
		pcapfile.setString(pcap_file_to_read.c_str());
	}

	//-------------------------------------------------------------------
	gui::label pcaplabel(50, 24);
	pcaplabel.setFont(mnufont);
	pcaplabel.setString("File:");
	pcapopts p(packetlist, &pcapfile, &mnu);

	/*--------------------------------------------------------------
	 * this is load button which tries to load the pcap file
	 */
	gui::button btnpcapfile(DATADIR "mnubg.png", DATADIR "mnusel.png", newpcapfile, &p, 1);
	btnpcapfile.setFont(mnufont);
	btnpcapfile.setString("Load");
	gui::layout capmenu;
	capmenu.addchild(pcaplabel, 0, 1);
	capmenu.addchild(pcapfile, 70, 1);
	capmenu.addchild(btnpcapfile, 35, 26);
	capmenu.hide();
	capmenu.hasborder = 1;
	gui::option mnupcapfile(DATADIR "mnubg.png", DATADIR "mnusel.png", mnutoggle, &capmenu, 1);
	mnupcapfile.setFont(mnufont);
	mnupcapfile.setString("Pcap File");
	menu.addchild(mnupcapfile, 200, 0);
	menu.addchild(capmenu, 200, 30);


	gui::layout widgets;
	widgets.addchild(mnu, 4, 0);
	widgets.addchild(menu, 28, 0);
	menu.hide();
	widgets.focus();

	//our little scrolling chart
	chart histo;

	//construct the popup menu
	gui::font popupfont;
	popupfont.setSize(14);
	gui::option resolve(DATADIR "cbunsel.png", DATADIR "cbsel.png", togglebool, NULL, 0);
	resolve.setFont(popupfont);
	resolve.setString("    Show IP");
	gui::option pin(DATADIR "cbunsel.png", DATADIR "cbsel.png", togglebool, NULL, 0);
	pin.setFont(popupfont);
	pin.setString("    Pin in place");
	gui::layout popupmenu;
	popupmenu.addchild(resolve, 0, 0);
	popupmenu.addchild(pin, 0, 18);
	vector<cxargs*> customargs;
	for (i = 0; i < customcommands.size(); i++)
	{
		cxargs* a = new cxargs;
		a->s = customcommands[i].c_str();
		a->ip = 0;
		customargs.push_back(a);
		gui::button*b = new gui::button(DATADIR "plainpopupitem.png", DATADIR "plainpopupitem.png", customexecute, (void*)a, 0);
		b->setFont(popupfont);
		b->setString(customlabels[i].c_str());
		popupmenu.addchild(*b, 0, 36 + i*18);
	}

			
	popupmenu.hide();
	


	if (speed == "")
	{
		speed = "1000";
	}



	//and lastly, try to connect to the server
	if (server != "")
	{
		SDL_WM_SetCaption((server +":"+ opts["filter"]).c_str(), "Oip");
		packetlist = new clientpm(server.c_str(), opts, port);
	}
	double ti = now();
	int chartheight = 128;
	int px, py;

	if (pcap_file_to_read != "") {
		btnpcapfile.activate();
	}

	bool pause = false;


	while(run)
	{
	
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			//cout << "something has happened\n";
			switch (event.type)
			{
			case SDL_QUIT: 
				run = false;
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == 'p'){
					cout << "OIPGUI.CPP: A keyboard -p- was pressed.\n";
					if(pause == false){
						pause = true;
					}else{
						pause = false;
					}
				}

				if (event.key.keysym.sym == SDLK_ESCAPE)
					run = false;

				if (event.key.keysym.sym == '!') 
					mnu.activate();

				else if (event.key.keysym.sym == ',')
					btnpcapfile.activate();
				else
					widgets.keydown(event.key);
				break;
			case SDL_KEYUP:
				widgets.keyup(event.key);
				break;
			case SDL_VIDEORESIZE:
				resize(event.resize.w, event.resize.h);
				chartheight = event.resize.h/3;
				histo.resize(event.resize.w, chartheight);
				break;
			case SDL_MOUSEBUTTONDOWN:
				//give the widgets the first crack at events
				if (!widgets.mousedown(event.button))
				{
					if (event.button.button == SDL_BUTTON_RIGHT)
					{
						entity* e = pm.bycoords(event.button.x, event.button.y);
						/*
						if (e)
							e->moving = !e->moving;
						if (e)
							e->resolve = !e->resolve;
						 */
						if (e)
						{

							px = event.button.x;
							py = event.button.y;

							pm.mousedown(event.button.x, event.button.y);
							resolve.setstate(!e->resolve);
							pin.setstate(!e->moving);
							resolve.arg = &e->resolve;
							pin.arg = &e->moving;
							int i;
							for (i = 0; i < customargs.size(); i++)
								customargs[i]->ip = e->label;
							popupmenu.show();
						}
						else
							popupmenu.hide();
					}
					else if (event.button.button == SDL_BUTTON_LEFT)
					{
						cout << "OIPGUI.CPP: i caught a click!\n";
						cout << "left click\n";
						//let the popup menu have it, if its there
						if (popupmenu.shown())
						{
							event.button.x -= px;
							event.button.y -= py;
							if (!popupmenu.mousedown(event.button))
								popupmenu.hide(); //hide if not clicked on
						}
						else
							pm.mousedown(event.button.x, event.button.y);
					}
				}
				break;
			case SDL_MOUSEMOTION:
				if (!widgets.mousemove(event.motion))
					pm.mousemove(event.motion.x, event.motion.y);
				break;
			case SDL_MOUSEBUTTONUP:
				if (popupmenu.shown() && event.button.button == SDL_BUTTON_LEFT)
				{
					event.button.x -= px;
					event.button.y -= py;
					popupmenu.mouseup(event.button);
					popupmenu.hide();
				}
				else
					if (!widgets.mouseup(event.button))
						pm.mouseup();
				break;
			}
		}


		if(pause!=true){
			double dt = now() - ti;
			ti = now();
			//cout << packetlist;
			//cout << "\n";
			if (packetlist)
			{
				packetlist->copydata(histo);
				//packetlist->dumpdata(pm);
				int numb;
				istringstream ( speed ) >> numb;
				packetlist->dumpdata(pm, numb);
			}
			pm.process(dt);

			// Black background
			SDL_FillRect(screen, NULL, 0);

			// White background
			//SDL_FillRect(screen, NULL, 4294967295);


			if (packetlist)
				histo.draw(0, screen->h - chartheight, screen);
			pm.draw(screen);
			/*
			stringstream ss;
			ss << 1.0/dt << " fps";
			text.render(screen, ss.str().c_str(), 0, 0, 8);
			// */
			widgets.draw(0, 0, screen);
			popupmenu.draw(px, py, screen);
			SDL_Flip(screen);
			//don't bother going faster than twice the minimum framerate
			if (dt * 1000 < MINRATE)
				SDL_Delay(MINRATE - (int)(dt * 1000));
		}
	}

	if (packetlist)
		delete packetlist;
	return 0;
}

