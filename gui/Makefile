LDFLAGS= -lcryptopp -lSDL -lSDL_image -lpcap `freetype-config --libs`
CPPFLAGS = -g `freetype-config --cflags` 

widgets = font.o widget.o label.o ../image.o textbox.o layout.o button.o option.o

guitest: ${widgets} guitest.o
	g++ ${LDFLAGS} ${widgets} guitest.o -o guitest
clean:
	rm -f *.o


