LDFLAGS=-lGL -lGLU -lSDL -lSDL_image -lpcap -lSDL_net -lXext
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

test: clean testclientmanager 
	./testclientmanager

clean:
	rm -f *.o

distclean: clean
	rm -f display

.PHONY: clean distclean $(COMMON)/flowdata.o

