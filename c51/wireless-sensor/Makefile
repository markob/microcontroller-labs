CC	:=sdcc
CFALGS	:=-mcs51 --std-sdcc99 --model-small --iram-size 256 --verbose -Wall -Werror
LDFLAGS	:=--out-fmt-ihx
RM	:=rm -rf

PROJDIR	:=$(shell pwd)

TARGET	:=sensor.hex

SOURCES	:=$(notdir $(wildcard $(PROJDIR)/*.c))
OBJECTS	:=$(SOURCES:.c=.rel)

$(TARGET):$(OBJECTS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJECTS) -o $@

%.rel: %.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(TARGET) *.lst *.rst *.map *.mem *.sym *.lk *.asm
