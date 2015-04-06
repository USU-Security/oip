AUTOMAKE_OPTIONS=subdir-objects

coreobj=config.cpp packetmanager.cpp clientpm.cpp clientmanager.cpp messages.cpp namecache.cpp encrypt.cpp hexprint.cpp myconfig.h packetmanager.h clientpm.h clientmanager.h messages.h namecache.h encrypt.h hexprint.h ips.h packetsink.h packetpeek.h networkpm.h

guiobj=particlemanager.cpp kdtree.cpp entity.cpp entityset.cpp image.cpp text.cpp particle.cpp chart.cpp capreader.cpp particlemanager.h kdtree.h entity.h entityset.h image.h text.h particle.h chart.h capreader.h intervalreader.h

widgets=gui/font.cpp gui/widget.cpp gui/label.cpp gui/textbox.cpp gui/layout.cpp gui/button.cpp gui/option.cpp gui/base.h gui/font.h gui/widget.h gui/label.h gui/textbox.h gui/layout.h gui/button.h gui/option.h gui/draw.h

oip_datadir = $(datadir)/@PACKAGE@/
oip_docdir = $(datadir)/doc/@PACKAGE@/

oipprgdir=$(prefix)/bin/
oipprg_PROGRAMS=oip
oip_SOURCES = $(coreobj) $(guiobj) $(widgets) oipgui.cpp

#oip_LDADD = $(am_oip_LIBS) $(FT2_LIBS) -lfontconfig
oip_LDADD = $(FT2_LIBS)
oip_CXXFLAGS = $(FT2_CFLAGS) -DDATADIR=\"$(oip_datadir)\"
#oip_LDFLAGS = `pkg-config --libs-only-L free`
#AM_CXXFLAGS = `freetype-config --cflags`
oipdprgdir=$(prefix)/bin/
oipdprg_PROGRAMS = oipd
oipd_SOURCES = $(coreobj) oipd.cpp structs.h

EXTRA_DIST = oip.conf-example *.png *.ttf

oip_data_DATA = oip.conf-example *.png *.ttf
oip_doc_DATA = oip.conf-example

