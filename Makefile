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
	

capreader: capreader.o particle.o text.o image.o kdtree.o particlemanager.o entityset.o entity.o packetmanager.o



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
	rm -f display

.PHONY: clean distclean $(COMMON)/flowdata.o

