CC = g++
LD = $(CC)
GITBINARY = git
FIRMWAREREPO = simulator_libraries
FEHURL = code.osu.edu

.PHONY: clean

CPPFLAGS = -MMD -MP -Os -DOBJC_OLD_DISPATCH_PROTOTYPES -g

WARNINGS = -Wall -Wno-c++11-extensions -Wno-unused-private-field -Wno-return-type -Wno-deprecated

INC_DIRS = -I$(LIB_DIR) -I. -I../../

OBJS = \
$(LIB_DIR)/Component.o \
$(LIB_DIR)/FEHAccel.o \
$(LIB_DIR)/FEHBattery.o \
$(LIB_DIR)/FEHBuzzer.o \
$(LIB_DIR)/FEHDashboard.o \
$(LIB_DIR)/FEHIO.o \
$(LIB_DIR)/FEHLCD.o \
$(LIB_DIR)/FEHMotor.o \
$(LIB_DIR)/FEHRandom.o \
$(LIB_DIR)/FEHRPS.o \
$(LIB_DIR)/FEHSD.o \
$(LIB_DIR)/FEHServo.o \
$(LIB_DIR)/FEHUtility.o \
$(LIB_DIR)/tigr.o \
$(LIB_DIR)/FEHThread.o \
$(LIB_DIR)/FEHMain.o \
../../main.o

ifeq ($(OS),Windows_NT)
	LDFLAGS = -lopengl32 -lgdi32
	EXEC = ..\..\simulator.exe
	SHELL := CMD
	LIB_DIR = Libraries
else
	LDFLAGS = -framework OpenGL -framework Cocoa
	EXEC = ../../simulator.out
	LIB_DIR = Libraries
endif

SRC_FILES := $(wildcard ./*.cpp)
OBJ_FILES := $(patsubst ./%.cpp,./%.o,$(SRC_FILES))

all: $(EXEC)

$(EXEC): $(OBJ_FILES) $(OBJS)
	$(CC) $(CPPFLAGS) $(WARNINGS) $(INC_DIRS) $(OBJ_FILES) $(OBJS) -pthread --std=c++11 -o $(EXEC) $(LDFLAGS)

./%.o: ./%.cpp
	$(CC) $(CPPFLAGS) $(WARNINGS) $(INC_DIRS) -pthread -std=c++11 -c -o $@ $<

clean:
ifeq ($(OS),Windows_NT)
	del $(LIB_DIR)\*.o $(LIB_DIR)\*.d $(EXEC)
else
	rm -f $(LIB_DIR)/*.o $(LIB_DIR)/*.d
	rm -f $(EXEC)
endif