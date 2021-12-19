# -*- mode: makefile-gmake-*-
# user defines ################################################################
TARGET = main
MCU = msp430fr6972
INC = -I.
OUTDIR = output/target
DEFINES =
DEBUG = -Os -g2 -ggdb -gdwarf-2
CFLAGS = $(DEBUG) -fno-exceptions -fno-unwind-tables $(DEFINES) -mmcu=$(MCU) -Wunused -MMD -MP -MMD -Wall -Wno-main
CXXFLAGS = $(DEBUG) -fno-exceptions -fno-unwind-tables -fno-use-cxa-atexit $(DEFINES) -mmcu=$(MCU) -MMD -MP -MMD -Wall -Wno-main -std=c++20
# ASFLAGS = -mmcu=$(MCU) -x assembler-with-cpp -Wa,-gstabs
LDFLAGS = $(DEBUG) -fno-exceptions -fno-unwind-tables -fno-use-cxa-atexit -mmcu=$(MCU) -T msp430fr6972.ld -Wl,-Map=$(OUTDIR)/$(TARGET).map,--gc-sections
#/nix/store/2n0civyah576zdl52yprs0nzb3d1cm2p-msp430-elf-stage-final-gcc-debug-10.3.0/msp430-elf/lib/libstdc++.a
# calculated vars #############################################################
SRC = $(TARGET).cpp
CFILES = $(filter %.c, $(SRC))
CPPFILES = $(filter %.cpp, $(SRC))
COBJECTS = $(addprefix $(OUTDIR)/,$(CFILES:.c=.o))
CPPOBJECTS = $(addprefix $(OUTDIR)/,$(CPPFILES:.cpp=.o))
OBJECTS = $(COBJECTS) $(CPPOBJECTS)

$(info   COBJECTS is $(COBJECTS))
$(info   CPPOBJECTS is $(CPPOBJECTS))
$(info   CFILES is $(CFILES))

# binaries ####################################################################
CC      	= msp430-elf-gcc
CXX			= msp430-elf-g++
OBJCOPY 	= msp430-elf-objcopy
GD          = msp430-elf-gdb
SZ          = msp430-elf-size
RM      	= rm -rf
MKDIR		= mkdir -p

# user targets ################################################################
.PHONY: all clean program debug program-noload size

all: $(OUTDIR)/$(TARGET).bin

include ../common/sources.mk
include hal/sources.mk

clean:
	$(RM) output

program: $(OUTDIR)/$(TARGET).elf
	mspdebug tilib "prog $<"

debug : $(OUTDIR)/$(TARGET).elf
	$(GD) -x init.gdb $<

program-noload : $(OUTDIR)/$(TARGET)_noload.elf
	mspdebug tilib "prog $<"

size : $(OBJECTS)
	$(SZ) $^ | sort -n

# internal targets ############################################################
$(OUTDIR)/$(TARGET).bin : $(OUTDIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(OUTDIR)/$(TARGET)_noload.elf : $(OUTDIR)/$(TARGET).elf
	$(OBJCOPY) --set-section-flags .infoMEM=noload $< $@

$(OUTDIR)/$(TARGET).elf: $(sort $(OBJECTS))
	$(CXX) -fno-exceptions $(OBJECTS) $(LDFLAGS) $(LIBS) -fno-exceptions -o $@ 
#-L/home/lennart/pluto-fw/target/msp430/msp430-elf/lib 
$(COBJECTS): $(OUTDIR)/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) -c $(CFLAGS) $(INC) -o $@ $<

$(CPPOBJECTS): $(OUTDIR)/%.o: %.cpp
	mkdir -p $(shell dirname $@)
	$(CXX) -c $(CXXFLAGS) $(INC) -o $@ $<

-include  $(OBJECTS:.o=.d)

# debug targets ###############################################################
print-% :
	@echo $* = $($*)
