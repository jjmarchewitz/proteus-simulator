CC = g++
LD = $(CC)

TARGET = main

CPPFLAGS = -MMD -MP -Os -DOBJC_OLD_DISPATCH_PROTOTYPES -g

IGNORED_WARNINGS = -Wall

LIB_DIR = Libraries

INC_DIRS = -I$(LIB_DIR) -I.

OBJS = $(LIB_DIR)/FEHLCD.o $(LIB_DIR)/FEHRandom.o $(LIB_DIR)/FEHSD.o $(LIB_DIR)/tigr.o

ifeq ($(OS),Windows_NT)
	LDFLAGS = -lopengl32 -lgdi32
	EXEC = $(TARGET).exe
else
	LDFLAGS = -framework OpenGL -framework Cocoa
	EXEC = $(TARGET).out
endif

$(TARGET): $(TARGET).o $(OBJS)
	$(CC) $(CPPFLAGS) $(IGNORED_WARNINGS) $(INC_DIRS) $(OBJS) $(TARGET).o -o $(EXEC) $(LDFLAGS) 

$(TARGET).o: $(TARGET).cpp $(TARGET).hpp 
	$(CC) $(IGNORED_WARNINGS) $(INC_DIRS) -c $(TARGET).cpp

FEHLCD.o: $(LIB_DIR)/FEHLCD.cpp $(LIB_DIR)/FEHLCD.h $(LIB_DIR)/FEHUtility.o
	$(CC) $(IGNORED_WARNINGS) $(INC_DIRS) -c $(LIB_DIR)/FEHLCD.cpp

FEHUtility.o: $(LIB_DIR)/FEHUtility.cpp $(LIB_DIR)/FEHUtility.h
	$(CC) $(IGNORED_WARNINGS) $(INC_DIRS) -c $(LIB_DIR)/FEHUtility.cpp

FEHRandom.o: $(LIB_DIR)/FEHRandom.cpp $(LIB_DIR)/FEHRandom.h
	$(CC) $(IGNORED_WARNINGS) $(INC_DIRS) -c $(LIB_DIR)/FEHRandom.cpp

FEHSD.o: $(LIB_DIR)/FEHSD.cpp $(LIB_DIR)/FEHSD.h
	$(CC) $(IGNORED_WARNINGS) $(INC_DIRS) -c $(LIB_DIR)/FEHSD.cpp

tigr.o: $(LIB_DIR)/tigr.c $(LIB_DIR)/tigr.h
	$(CC) $(IGNORED_WARNINGS) $(INC_DIRS) -c $(LIB_DIR)/tigr.c

clean:
	rm $(LIB_DIR)/*.o $(LIB_DIR)/*.d
	rm *.o $(EXEC)