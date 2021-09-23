CC = g++
LD = $(CC)

TARGET ?= game

CPPFLAGS ?= -v -MMD -MP -Os -DOBJC_OLD_DISPATCH_PROTOTYPES

ifeq ($(OS),Windows_NT)
	LDFLAGS = -lopengl32 -lgdi32
else
	LDFLAGS = -framework OpenGL -framework Cocoa
endif

# I nuked the old makefile and the entire file structure while attempting to eliminate variables as to why I couldn't build :D
# If you know how to upgrade the makefile feel free to do so!
game: game.o FEHLCD.o tigr.o
	g++ $(CPPFLAGS) game.o FEHLCD.o tigr.o -o game.exe $(LDFLAGS)

game.o: game.cpp game.hpp
	g++ -Wall -c game.cpp

FEHLCD.o: FEHLCD.cpp FEHLCD.h
	g++ -Wall -c FEHLCD.cpp

tigr.o: tigr.c tigr.h
	g++ -Wall -c tigr.c

clean:
	rm *.o game.o