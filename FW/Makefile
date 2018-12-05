# Makefile
#
# targets:
#   all:    compiles the source code
#   isp-info:   tests the isp connection to the mcu
#   flash:  writes compiled hex file to the mcu's flash memory
#   fuse:   writes the fuse bytes to the MCU
#   disasm: disassembles the code for debugging
#   clean:  removes all .hex, .elf, and .o files in the source code and library directories

# parameters (change this stuff accordingly)
# project name
PRJ = xmas
# avr mcu
MCU = attiny102
# mcu clock frequency
CLK = 1000000UL
# avr programmer (and port if necessary)
# e.g. PRG = usbtiny -or- PRG = arduino -P /dev/tty.usbmodem411
PRG = avrispmkII
# fuse values for avr: byte
# these values are from an Arduino Uno (ATMega328P)
# see http://www.engbedded.com/fusecalc/ for other MCUs and options
FBYTE = 0xFF
# program source files (not including external libraries)
SRC = main.c
# where to look for external libraries (consisting of .c/.cpp files and .h files)
# e.g. EXT = ../../EyeToSee ../../YouSART
#EXT = ./uart

# include path
INCLUDE := $(foreach dir, $(EXT), -I$(dir))
# c flags - defaults
CFLAGS = -Wall -O1
CFLAGS += -DF_CPU=$(CLK) -ffunction-sections -fdata-sections -fpack-struct -fshort-enums
CFLAGS += -mmcu=$(MCU)
CFLAGS += -B /home/james/.atmel-packs/Atmel.ATtiny_DFP.1.3.229/gcc/dev/attiny102
CFLAGS += -I /home/james/.atmel-packs/Atmel.ATtiny_DFP.1.3.229/include
CFLAGS += $(INCLUDE)
# uart library specific flags - see https://github.com/andygock/avr-uart
CFLAGS += -std=gnu99

# executables
AVRDUDE = avrdude -c $(PRG) -p $(MCU)
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE    = avr-size --format=avr --mcu=$(MCU)
CC      = avr-gcc

# generate list of objects
CFILES    = $(filter %.c, $(SRC))
EXTC     := $(foreach dir, $(EXT), $(wildcard $(dir)/*.c))
OBJ       = $(CFILES:.c=.o) $(EXTC:.c=.o)

# user targets
# compile all files
all: $(PRJ).hex

# check programmer connectivity
isp-info:
	$(AVRDUDE) -v

# flash program to mcu
flash: all
	$(AVRDUDE) -U flash:w:$(PRJ).hex:i

# write fuses to mcu
fuse:
	$(AVRDUDE) -U BYTE0:w:$(FBYTE):m

# generate disassembly files for debugging
disasm: $(PRJ).elf
	$(OBJDUMP) -d $(PRJ).elf

# remove compiled files
clean:
	rm -f *.hex *.elf *.o
	$(foreach dir, $(EXT), rm -f $(dir)/*.o;)

# other targets
# objects from c files
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

# elf file
$(PRJ).elf: $(OBJ)
	$(CC) $(CFLAGS) -o $(PRJ).elf $(OBJ)

# hex file
$(PRJ).hex: $(PRJ).elf
	rm -f $(PRJ).hex
	$(OBJCOPY) -j .text -j .data -O ihex $(PRJ).elf $(PRJ).hex
	$(SIZE) $(PRJ).elf
