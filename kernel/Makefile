# Date:2017/6/8 2:21
# Author:HuWenjie<1@hwj.me>
# Ghost Bird File System Reader
# Makefile
# 
# BSD 2-Clause License
# 
# Copyright (c) 2017, Ghost Bird Operating System Project Developers.
# All rights reserved.

DIRS := $(shell find . -maxdepth 9 -type d)

C_SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
CPP_SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
ASM_SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.asm))

OBJS := $(patsubst %.c, %.o, $(C_SRCS))
OBJS += $(patsubst %.cpp, %.o, $(CPP_SRCS))
OBJS += $(patsubst %.asm, %.o, $(ASM_SRCS))

OBJCOPYFLAGS := -R .note -R .comment -S

LDFLAGS  := -m elf_i386
CFLAGS   := -fno-stack-protector -I $(CURDIR)/include
CPPFLAGS := -fno-stack-protector -I $(CURDIR)/include
ASMFLAGS := -f elf

TARGET = KERNEL

sinclude $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): kernel.o
	$(OBJCOPY) -O binary kernel.o $(TARGET)

kernel.o: $(OBJS)
	$(LD) $(LDFLAGS) -o kernel.o -Ttext 0x10000 $(OBJS)

%.o: %.asm
	$(NS) $(ASMFLAGS) -o $@ $<

%.d: %.asm
	@$(NS) -M $(ASMFLAGS) $< > $@; \
	sed -i 's,^:,$*.o $@:,g' $@;

%.d: %.c
	@$(CC) -M $(CFLAGS) $< > $@; \
	sed -i 's,$(notdir $*)\.o[ :]*,$*.o $@ : ,g' $@;

%.d: %.cpp
	@$(CXX) -M $(CPPFLAGS) $< > $@; \
	sed -i 's,$(notdir $*)\.o[ :]*,$*.o $@ : ,g' $@;

clean:
	$(RM) $(TARGET) kernel.o *.exe $(OBJS) $(OBJS:.o=.d)
