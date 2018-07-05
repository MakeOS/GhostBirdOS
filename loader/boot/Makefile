# Date:Miss
# Author:HuWenjie<1@hwj.me>
# Explorer Loader
# makefile for Explorer Loader Boot
# 
# BSD 2-Clause License
# 
# Copyright (c) 2017, Ghost Bird Operating System Project Developers.
# All rights reserved.

# Note that there is a boota.asm, which is boota.o in the first element
# of $(OBJ), because it has entry "_start"

DIRS := $(shell find . -maxdepth 9 -type d)
C_SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
ASM_SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.asm))

OBJS := $(patsubst %.c, %.o, $(C_SRCS))
OBJS += $(patsubst %.asm, %.o, $(ASM_SRCS))

OBJCOPYFLAGS := -R .note -R .comment -S

LDFLAGS  := -m elf_i386
CFLAGS   := -fno-stack-protector -I $(CURDIR)/include
ASMFLAGS := -f elf

TARGET = BOOT

sinclude $(OBJS:.o=.d)

install:BOOT
	$(gfr) -ws $(TARGET) $(image) 2

all: $(TARGET)
	
# compile files
	
$(TARGET):setup.bin boot.bin
	cat setup.bin boot.bin > $(TARGET)
	
boot.bin:boot.o
	$(OBJCOPY) -O binary boot.o boot.bin
	
boot.o:$(OBJS)
	$(LD) $(LDFLAGS) -o $@ -Ttext 0x11000 $(OBJS)

setup.bin:setup.basm
	nasm setup.basm -o setup.bin

%.o: %.asm
	$(NS) $(ASMFLAGS) -o $@ $<

%.d: %.asm
	@$(NS) -M $(ASMFLAGS) $< > $@; \
	sed -i 's,^:,$*.o $@:,g' $@;

%.d: %.c
	@$(CC) -M $(CFLAGS) $< > $@; \
	sed -i 's,$(notdir $*)\.o[ :]*,$*.o $@ : ,g' $@;

clean:
	$(RM) BOOT $(OBJS) $(OBJS:.o=.d) boot.o *.bin

