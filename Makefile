INCDIR = /usr/include/qt3
CFLAGS = -O2 -ansi -DNDEBUG -fno-strength-reduce -Wall -W -I/usr/X11R6/include
LIBCFLAGS = -fPIC 
YACCCFLAGS = -Wno-unused -Wno-parentheses
LFLAGS = -L/usr/lib -lqt-mt -lm -L/usr/X11R6/lib -lX11
CC = g++
MOC = /usr/share/qt3/bin/moc
SHELL =	/bin/sh


####### Files

HEADERS =	nplot.h        canvas.h        menu.h        butgr.h        transfo.h 	
SOURCES =	nplot.cpp      canvas.cpp	 menu.cpp      butgr.cpp      transfo.cpp   main.cpp  
OBJECTS =	nplot.o        canvas.o	       menu.o        butgr.o        transfo.o     main.o    
SRCMETA =	nplot.moc.cpp  canvas.moc.cpp  menu.moc.cpp  butgr.moc.cpp  	
OBJMETA =	nplot.moc.o    canvas.moc.o	 menu.moc.o    butgr.moc.o    
TARGET  =	nodplot

####### Implicit rules

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(TARGET)

$(TARGET): $(OBJECTS) $(OBJMETA)
	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS)

depend: $(SRCMETA)
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(SOURCES) $(HEADERS) Makefile

clean:
	-rm -f *.o *.bak *BAK *~ *% #*
	-rm -f $(SRCMETA) $(TARGET)
     

####### Meta objects

canvas.moc.cpp: canvas.h
	$(MOC) -o canvas.moc.cpp canvas.h

menu.moc.cpp: menu.h
	$(MOC) -o menu.moc.cpp menu.h

butgr.moc.cpp: butgr.h
	$(MOC) -o butgr.moc.cpp butgr.h

nplot.moc.cpp: nplot.h
	$(MOC) -o nplot.moc.cpp nplot.h



# DO NOT DELETE THIS LINE -- make depend depends on it.
