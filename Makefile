# Date:2017/6/20 16:10
# Author:HuWenjie<1@hwj.me>
# Ghost Bird OS
# Makefile
# 
# BSD 2-Clause License
# 
# Copyright (c) 2017, Ghost Bird Operating System Project Developers.
# All rights reserved.

# There are some command different among OS
platform = $(win)
windows = win
linux = linux

ifeq ($(platform), $(linux))
	clean_screen = clear
else ifeq ($(platform), $(windows))
	clean_screen = cls
	RM = del
endif



.PHONY:gfr
.PHONY:loader
.PHONY:help
.PHONY:clean
.PHONY:run
.PHONY:all

gfr:
	cd "tools/Ghost Bird File System Reader" && make all
	
loader:
	cd loader && make all

help:
	$(clean_screen)
	@echo Ghost Bird Project Makefile - Help
	@echo target		introduction
	@echo gfr		make Ghost Bird File System Reader
	@echo loader		make Explorer Loader
	@echo clean		make cl
	@echo loader		make Explorer Loader

dist : clean
	makecab image/image.vhd
	tar czf 02152c.tar.gz *.c Makefile
	
clean:
	$(RM) *.o *.zip

run:

all:
