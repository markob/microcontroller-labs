TGTDEFS	:=-mmcs51 --model-small --iram-size 256 --code-size 4096

CC	:=sdcc
CFLAGS	:=$(TGTDEFS) --std-sdcc99 --opt-code-speed --verbose
LDFLAGS	:=--out-fmt-ihx
RM	:=rm -rf

PROJDIR	:=$(shell pwd)

TARGET	:=sensor.hex

SOURCES	:=$(notdir $(wildcard $(PROJDIR)/*.c))
OBJECTS	:=$(SOURCES:.c=.rel)

$(TARGET):$(OBJECTS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJECTS) -o $@

%.rel:%.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(TARGET) *.lst *.rst *.map *.mem *.sym *.lk *.asm
