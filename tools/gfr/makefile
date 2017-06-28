# Date:2017/6/8 2:21
# Author:HuWenjie<1@hwj.me>
# Ghost Bird File System Reader
# Makefile
# 
# BSD 2-Clause License
# 
# Copyright (c) 2017, Ghost Bird Operating System Project Developers.
# All rights reserved.

CC			:= g++
CPPFILES	:= gfr.cpp im/im.cpp im/vhd.cpp
OBJ			:= $(CPPFILES:.cpp=.o)

EXECUTE := gfr.exe
LDFLAGS := -Wall -O2
CPPFLAGS := -I C:/MinGW/include

EXECUTE: $(OBJ)
	$(LD) -o $(LDFLAGS) $(EXECUTE) $(OBJ)

all: EXECUTE
	
.c.o:
	$(CC) -c $< $(CFLAGS)
	
	
clean:
	$(RM) gfr.exe
