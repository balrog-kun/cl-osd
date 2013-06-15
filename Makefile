# Basic makefile for cl-osd contributed by davico(rcgroups).
# Tested with r65 on ubuntu 11.10.
# Note that this may not be up-to-date. YMMV.

CC=avr-gcc
CFLAGS=-funsigned-char -funsigned-bitfields -Os -fno-split-wide-types -g0 -Wall -std=gnu99 -Wno-main -mmcu=atmega88

all: cl-osd.hex

cl-osd.hex cl-osd.eep: cl-osd
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .bss -R .stab -R .stabstr "cl-osd" "cl-osd.hex"
	avr-objdump -h -S "cl-osd" > "cl-osd.lss"
	avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex "cl-osd" "cl-osd.eep" || exit 0   

cl-osd.c: *.h
cl-osd: cl-osd.c

clean:
	-rm -f cl-osd cl-osd.o cl-osd.lss cl-osd.hex cl-osd.eep

upload: cl-osd.hex cl-osd.eep
	# Note we're using -F (force) and could be overwriting something
	avrdude -c avrisp -b 19200 -P /dev/ttyUSB0 -F -p m88 -v -e -U flash:w:cl-osd.hex
	avrdude -c avrisp -b 19200 -P /dev/ttyUSB0 -F -p m88 -v -U eeprom:w:cl-osd.eep
