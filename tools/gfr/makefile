# Date:2017/6/8 2:21
# Author:HuWenjie<1@hwj.me>
# Ghost Bird File System Reader
# Makefile
# 
# BSD 2-Clause License
# 
# Copyright (c) 2017, Ghost Bird Operating System Project Developers.
# All rights reserved.

CC			:= gcc
DIRS		:= $(CURDIR) fs bf io
CFILES		:= $(foreach dir,$(DIRS),$(wildcard $(dir)/*.c))
OBJ			:= $(CFILES:.c=.o)

EXECUTE := gfr.exe
CFLAGS := -Wall -O2 -I $(CURDIR)/include/

.PHONY: $(EXECUTE)

$(EXECUTE):all
	$(CC) -o $(EXECUTE) $(OBJ)

all:
	make $(OBJ)

%.d:%.c
	@gcc -M $(CFLAGS) $< -o $@

%.o:%.c
	@gcc -M $(CFLAGS) $< -o $(<:.c=.d)
	gcc -c $(CFLAGS) $< -o $@
	
clean:
	$(RM) $(EXECUTE)
	$(RM) $(CFILES:.c=.d)
	$(RM) $(OBJ)

-include $(CFILES:.c=.d)
