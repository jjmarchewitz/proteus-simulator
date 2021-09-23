CC = g++
LD = $(CC)

TARGET ?= game

CPPFLAGS ?= -MMD -MP -Os -DOBJC_OLD_DISPATCH_PROTOTYPES

LDFLAGS ?= -framework OpenGL -framework Cocoa

game: game.o FEHLCD.o tigr.o
	g++ $(CPPFLAGS) $(LDFLAGS) game.o FEHLCD.o tigr.o -o game

game.o: game.cpp 
	g++ -c game.cpp

FEHLCD.o: FEHLCD.cpp tigr.c
	g++ -c FEHLCD.cpp tigr.c

tigr.o: tigr.c
	g++ -c tigr.c

clean:
	rm *.o game