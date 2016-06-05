# -*- mode: makefile-gmake-*-
# user defines ################################################################
TARGET = main
MCU = msp430fr6972
INC = -I.
OUTDIR = output/target
DEFINES =
CFLAGS = $(DEFINES) -mmcu=$(MCU) -g3 -O3 -Wunused -ffunction-sections -MP -MMD #-Wall
# ASFLAGS = -mmcu=$(MCU) -x assembler-with-cpp -Wa,-gstabs
LDFLAGS = -mmcu=$(MCU) -Tmsp430fr6972.ld -Wl,-Map=$(OUTDIR)/$(TARGET).map,--gc-sections

# calculated vars #############################################################
SRC = $(TARGET).c
CFILES = $(filter %.c, $(SRC))
OBJECTS = $(addprefix $(OUTDIR)/,$(CFILES:.c=.o))

# binaries ####################################################################
CC      	= msp430-elf-gcc
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
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@

$(OBJECTS): $(OUTDIR)/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) -c $(CFLAGS) $(INC) -o $@ $<

-include  $(OBJECTS:.o=.d)

# debug targets ###############################################################
print-% :
	@echo $* = $($*)
