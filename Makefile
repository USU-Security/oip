LDFLAGS= -lcryptopp -lSDL -lSDL_image -lpcap
CPPFLAGS = -g 
all: capreader

text.o: text.h

entity.o: entity.h 

entityset.o: entityset.h

entitytest: entitytest.o entity.o text.o entityset.o

particletest: particletest.o particle.o text.o image.o kdtree.o

particlemanagertest: particlemanagertest.o particle.o text.o image.o kdtree.o particlemanager.o entityset.o entity.o

capreader: capreader.o particle.o text.o image.o kdtree.o particlemanager.o entityset.o entity.o packetmanager.o

testclientmanager: testclientmanager.o clientmanager.o packetmanager.o particlemanager.o kdtree.o entity.o entityset.o image.o text.o particle.o clientpm.o networkpm.o clientmanager.o messages.o 

objlist = config.o packetmanager.o particlemanager.o kdtree.o entity.o entityset.o image.o text.o particle.o clientpm.o networkpm.o clientmanager.o messages.o namecache.o encrypt.o hexprint.o

oipd: ${objlist} oipd.o
	g++ ${objlist} oipd.o ${LDFLAGS} -o oipd

oip: ${objlist} oip.o 
	g++ ${objlist} oip.o ${LDFLAGS} -o oip

test: clean testclientmanager 
	./testclientmanager

clean:
	rm -f *.o

distclean: clean
	rm -f display

.PHONY: clean distclean $(COMMON)/flowdata.o

