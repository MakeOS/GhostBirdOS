/**
 * Date:2017/6/26 17:57
 * Author:HuWenjie<1@hwj.me>
 * Ghost Bird File System Reader
 * defination of Class image handle
 * 
 * BSD 2-Clause License
 * 
 * Copyright (c) 2017, Ghost Bird Operating System Project Developers.
 * All rights reserved.
 **/
 
#ifndef IH_H_
#define IH_H_

class ImageHandle
{
public:
	~ImageHandle();
	int open(const char *parameter);
	int close(void);
	int put(int location, char c);
	int get(int location);
	int get_size(void);
	int init(int type);
	int format(int partition, int fs);
	int new_partition(int n, int start, int size);
private:
	string format;
	string path;
	FILE *image;
}

#endif
