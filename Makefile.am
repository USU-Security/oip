coreobj=config.cpp packetmanager.cpp clientpm.cpp clientmanager.cpp messages.cpp namecache.cpp encrypt.cpp hexprint.cpp myconfig.h packetmanager.h clientpm.h clientmanager.h messages.h namecache.h encrypt.h hexprint.h

guiobj=particlemanager.cpp kdtree.cpp entity.cpp entityset.cpp image.cpp text.cpp particle.cpp chart.cpp capreader.cpp particlemanager.h kdtree.h entity.h entityset.h image.h text.h particle.h chart.h capreader.h 

widgets=gui/font.cpp gui/widget.cpp gui/label.cpp gui/textbox.cpp gui/layout.cpp gui/button.cpp gui/option.cpp gui/base.h gui/font.h gui/widget.h gui/label.h gui/textbox.h gui/layout.h gui/button.h gui/option.h

oipprgdir=$(prefix)/bin/
oipprg_PROGRAMS=oip
oip_SOURCES = $(coreobj) $(guiobj) $(widgets) oipgui.cpp
oip_LDFLAGS = `freetype-config --libs`
AM_CXXFLAGS = `freetype-config --cflags`
oipdprgdir=$(prefix)/bin/
oipdprg_PROGRAMS = oipd
oipd_SOURCES = $(coreobj) oipd.cpp



