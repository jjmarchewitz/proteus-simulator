CC = g++
LD = $(CC)

TARGET ?= game
SRC_DIRS ?= ./lib ./lib/fehproteusfirmware ./lib/fehproteusfirmware/Libraries

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -Os -DOBJC_OLD_DISPATCH_PROTOTYPES

LDFLAGS ?= -framework OpenGL -framework Cocoa

$(TARGET): game.o $(OBJS)
	$(CC) $(LDFLAGS) game.o $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)