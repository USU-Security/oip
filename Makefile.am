coreobj=config.cpp packetmanager.cpp clientpm.cpp clientmanager.cpp messages.cpp namecache.cpp encrypt.cpp hexprint.cpp myconfig.h packetmanager.h clientpm.h clientmanager.h messages.h namecache.h encrypt.h hexprint.h ips.h packetsink.h packetpeek.h networkpm.h

guiobj=particlemanager.cpp kdtree.cpp entity.cpp entityset.cpp image.cpp text.cpp particle.cpp chart.cpp capreader.cpp particlemanager.h kdtree.h entity.h entityset.h image.h text.h particle.h chart.h capreader.h intervalreader.h

widgets=gui/font.cpp gui/widget.cpp gui/label.cpp gui/textbox.cpp gui/layout.cpp gui/button.cpp gui/option.cpp gui/base.h gui/font.h gui/widget.h gui/label.h gui/textbox.h gui/layout.h gui/button.h gui/option.h gui/draw.h

oipprgdir=$(prefix)/bin/
oipprg_PROGRAMS=oip
oip_SOURCES = $(coreobj) $(guiobj) $(widgets) oipgui.cpp
oip_LDFLAGS = `freetype-config --libs`
AM_CXXFLAGS = `freetype-config --cflags`
oipdprgdir=$(prefix)/bin/
oipdprg_PROGRAMS = oipd
oipd_SOURCES = $(coreobj) oipd.cpp structs.h


dist-hook:
	echo "#the shared secret used for encryption" >> $(distdir)/oip.conf
	echo "secret pleasepleasechangeme" >> $(distdir)/oip.conf
	echo "" >> $(distdir)/oip.conf
	echo "#Which IP addresses should be highlighted" >> $(distdir)/oip.conf
	echo "localnet 10.1.0.0" >> $(distdir)/oip.conf
	echo "localmask 255.255.0.0" >> $(distdir)/oip.conf
	echo "" >> $(distdir)/oip.conf
	echo "#These specify custom command options You can have more than one" >> $(distdir)/oip.conf
	echo "#the format for the string is Title_of_command shell command" >> $(distdir)/oip.conf
	echo "#the underscores in the title will be replaced by spaces in the gui" >> $(distdir)/oip.conf
	echo "#a single %s will be replaced by the ip address of the selected host" >> $(distdir)/oip.conf
	echo "#customcmd = Echo_the_ip echo \"user selected %s\" " >> $(distdir)/oip.conf
	echo "#customcmd = Popup_the_ip xmessage \"user selected %s\"" >> $(distdir)/oip.conf
	echo "#customcmd = Ping_the_ip ping %s -n -c 1" >> $(distdir)/oip.conf
	echo "" >> $(distdir)/oip.conf

install-exec-hook:
	if test -e /etc/oip.conf; then echo "Existing Configuration already found"; else cp $(srcdir)/oip.conf /etc/; fi

