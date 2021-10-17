CC = g++
LD = $(CC)

TARGET ?= game

CPPFLAGS ?= -v -MMD -MP -Os -DOBJC_OLD_DISPATCH_PROTOTYPES

IGNORED_WARNINGS ?= -Wall

ifeq ($(OS),Windows_NT)
	LDFLAGS = -lopengl32 -lgdi32
	EXEC = game.exe
else
	LDFLAGS = -framework OpenGL -framework Cocoa
	EXEC = game.out
endif

# I nuked the old makefile and the entire file structure while attempting to eliminate variables as to why I couldn't build :D
# If you know how to upgrade the makefile feel free to do so!
game: game.o FEHUtility.o FEHLCD.o FEHRandom.o FEHSD.o tigr.o
	g++ $(CPPFLAGS) game.o FEHLCD.o FEHRandom.o FEHSD.o tigr.o -o $(EXEC) $(LDFLAGS) $(IGNORED_WARNINGS)

game.o: game.cpp game.hpp
	g++ -Wall -c game.cpp

FEHLCD.o: FEHLCD.cpp FEHLCD.h
	g++ -Wall -c FEHLCD.cpp

FEHUtility.o: FEHUtility.cpp FEHUtility.h
	g++ -Wall -c FEHUtility.cpp

FEHRandom.o: FEHRandom.cpp FEHRandom.h
	g++ -Wall -c FEHRandom.cpp

FEHSD.o: FEHSD.cpp FEHSD.h
	g++ -Wall -c FEHSD.cpp

tigr.o: tigr.c tigr.h
	g++ -Wall -c tigr.c

clean:
	rm *.o game.out