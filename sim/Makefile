CC=gcc
CXX=g++
LD=ld 
AR=ar
AS=as
CP=objcopy
OD=objdump

EXECUTABLE=sim

all: $(EXECUTABLE)

SRC = hal/main.c hal/pa-beep.c

INC = -I./

include ../common/sources.mk

#  C source files
CFILES = $(filter %.c, $(SRC))
CXXFILES = $(filter %.cpp, $(SRC))

BUILDPREFIX=build/emu/

# Object files
COBJ = $(addprefix $(BUILDPREFIX),$(CFILES:.c=.o))
CXXOBJ = $(addprefix $(BUILDPREFIX),$(CXXFILES:.cpp=.o))
SOBJ = $(addprefix $(BUILDPREFIX),$(ASMFILES:.s=.o))
OBJ  = $(COBJ) $(SOBJ) $(CXXOBJ)

OPTIMIZE=-O0 -fdata-sections -ffunction-sections
DEBUG   = -g3
CFLAGS  = $(DEBUG) $(OPTIMIZE) $(shell pkg-config --cflags libpulse) -MP -MMD -pthread -Wall -Wno-main
CXXFLAGS = $(DEBUG) $(OPTIMIZE) $(shell pkg-config --cflags libpulse) -MP -MMD -pthread -Wall -Wno-main -std=c++20
LDFLAGS = -lzmq -lm $(shell pkg-config --libs libpulse) -pthread -lmbedcrypto

$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFLAGS) $(sort $(OBJ)) $(LDFLAGS) -o $@

$(COBJ): $(BUILDPREFIX)%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) -c $(DEFINES) $(INC) $(CFLAGS) $< -o $@

$(CXXOBJ): $(BUILDPREFIX)%.o: %.cpp
	mkdir -p $(shell dirname $@)
	$(CXX) -c $(DEFINES) $(INC) $(CXXFLAGS) $< -o $@

$(SOBJ): $(BUILDPREFIX)%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@

clean:
	rm -f $(OBJ) $(BIN) $(EXECUTABLE) $(COBJ:.o=.d) $(MAPS_C) $(MAPS_HEADER) $(MELODIES_C)

-include  $(COBJ:.o=.d)

.PHONY: clean
