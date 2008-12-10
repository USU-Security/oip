GUILDFLAGS= -lcryptopp -lSDL -lSDL_image `freetype-config --libs` -lpcap
SERVERLDFLAGS= -lcryptopp -lSDL -lpcap
CPPFLAGS = -g `freetype-config --cflags` 
all: oipgui oipd

text.o: text.h

entity.o: entity.h 

entityset.o: entityset.h

entitytest: entitytest.o entity.o text.o entityset.o

particletest: particletest.o particle.o text.o image.o kdtree.o

pmtobj = particlemanagertest.o particle.o text.o image.o kdtree.o particlemanager.o entityset.o entity.o namecache.o
particlemanagertest: ${pmtobj}
	g++ ${CPPFLAGS} ${pmtobj} ${LDFLAGS} -o particlemanagertest
	

testclientmanager: testclientmanager.o clientmanager.o packetmanager.o particlemanager.o kdtree.o entity.o entityset.o image.o text.o particle.o clientpm.o clientmanager.o messages.o 

coreobj = config.o packetmanager.o clientpm.o clientmanager.o messages.o namecache.o encrypt.o hexprint.o
guiobj = particlemanager.o kdtree.o entity.o entityset.o image.o text.o particle.o chart.o capreader.o 
widgets = gui/font.o gui/widget.o gui/label.o gui/textbox.o gui/layout.o gui/button.o gui/option.o

oipd: ${coreobj} oipd.o
	g++ ${coreobj} oipd.o ${SERVERLDFLAGS} -o oipd


oipgui: ${coreobj} ${guiobj} ${widgets} oipgui.o 
	g++ ${coreobj} ${guiobj} ${widgets} oipgui.o ${GUILDFLAGS} -o oipgui

guitest: guitest.o font.o

test: clean testclientmanager 
	./testclientmanager

clean:
	rm -f *.o gui/*.o

distclean: clean
	rm -f oipd oipgui testiptree testclientmanager particlemanagertest gui/guitest
dist: distclean
	mkdir .tmp
	mkdir .tmp/oip
	cp -r * .tmp/oip/
	rm -f .tmp/oip/oip.conf
	echo "#the shared secret used for encryption" >> .tmp/oip/oip.conf
	echo "secret pleasepleasechangeme" >> .tmp/oip/oip.conf
	echo "" >> .tmp/oip/oip.conf
	echo "#Which IP addresses should be highlighted" >> .tmp/oip/oip.conf
	echo "localnet 10.1.0.0" >> .tmp/oip/oip.conf
	echo "localmask 255.255.0.0" >> .tmp/oip/oip.conf
	echo "" >> .tmp/oip/oip.conf
	echo "#These specify custom command options You can have more than one" >> .tmp/oip/oip.conf
	echo "#the format for the string is Title_of_command shell command" >> .tmp/oip/oip.conf
	echo "#the underscores in the title will be replaced by spaces in the gui" >> .tmp/oip/oip.conf
	echo "#a single %s will be replaced by the ip address of the selected host" >> .tmp/oip/oip.conf
	echo "#customcmd = Echo_the_ip echo \"user selected %s\" " >> .tmp/oip/oip.conf
	echo "#customcmd = Popup_the_ip xmessage \"user selected %s\"" >> .tmp/oip/oip.conf
	echo "#customcmd = Ping_the_ip ping %s -n -c 1" >> .tmp/oip/oip.conf
	echo "" >> .tmp/oip/oip.conf
	find .tmp/oip/ -iname ".*" | xargs rm -rf
	tar -cvzf oip.tar.gz  -C .tmp oip
	rm -rf .tmp



testiptree: iptree.o testiptree.o ${coreobj} ${guiobj} 
	g++ ${CPPFLAGS} ${coreobj} ${guiobj} iptree.o testiptree.o ${GUILDFLAGS} -o testiptree

.PHONY: clean distclean $(COMMON)/flowdata.o

