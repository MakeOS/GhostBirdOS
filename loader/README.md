Copyright 2018 GhostBird OS

Lab Explorer Loader

GhostBird OS is licensed under the BSD 2-Clause "Simplified" License.

功能描述：这个程序用于从磁盘上提供包括引导探索者内核启动在内的多个功能。

Description: The program is used to boot Lab Explorer from Hard Disk, including other relevant functions.


配置要求：本引导程序运行在Intel x86 处理器架构上，要求处理器必须是80386或以上级别，内存最少不得少于4MB（不包括加载内核所需空间），IDE通道的硬盘上有至少一个FAT32活动分区，用于装载配置文件。如果有可能，请尽可能的在虚拟机中开启虚拟化技术VT-x/AMD-V。上述硬件配置要求并不一定是一成不变的，在后续的开发中一些限制可能会陆续放宽。

Requirements: The loader runs on Intel x86 architectures(at least 80386), requiring a memory of greater than 4MB. An IDE hard disk with a FAT32 active partition(used to storage configurations) is also needed. Please enable virtualization(such as VT-x/AMD-V) when running in virtual machine. The requirements mentioned above is not always still, and some restrictions may be liberalized in the further development.


提示：本引导程序可能没有经过综合检验，强烈建议本引导程序的使用者在虚拟机下使用本引导程序。


Notes: This program hasn't tested under complex circunstances. We insist that users should run the loader in the VMs.


版本描述：loader的版本信息仅在/comfigure.h中定义成version，一般格式为“A.b type c”，其中A为主版本号，b为次版本号，type为类型，通常有常规（General），开发（Building），内测（Alpha）以及公测（Beta）。

Version description: The version information is only defined as "version" in /comfigure.h, the normal format is "A.b type c".
A is the main version, b is the secondary version, type normally includes General/Building/Alpha/Beta.


文件描述：本引导程序的源代码文件夹中通常会充斥以下类型的文件：

File description: The source code folder is ordinarily filled with files with these types:

.C文件：C语言编写的源代码文件 Source code written in C

.h文件：C语言编写时预编译头文件 Header files when writting in C

.asm文件：汇编语言编写时的源代码文件 Source code written in Assembly Language

.O文件：编译时产生的目标文件 Object Files when compiling

.bin文件：无格式二进制编码文件 Raw Binary Files


本级结构：除一些全局性的文件之外，本级目录中存在探索者内核引导程序组成的两大部分，即主引导记录源代码部分（/mbr）和保留扇区源代码部分（/boot）部分。

Structure in this directory: Apart from global files, this class of directory contains the two main parts of kernel loader: Master Boot Record (/mbr) and source code used in reserved sectors(/boot).


提示：本引导程序的各级文件夹中大多数有针对本级源代码、本级文件、本级设计思路及实现原理的相关描述，敬请留意。

Notes: Please pay attention to READMEs, mostly distributed in every class of directories, which describe source code, files, designs and realization principles in the same class.


本目录下存在的文件或文件夹：

Folders and Files in this directory:

\boot：保留扇区的引导程序源代码文件夹 Folder of the source code of loader program in reserved sectors

\mbr：主引导记录的源代码文件夹 Folder of the MBR source code 

\configure.h：配置文件。以头文件的形式被引用 Configuration Files. (Refered as a header file)

\Makefile：提供全局的编译、调试描述。 It is just a makefile ;)

\README.md：本文件。用于描述、说明本引导程序。 You're reading.

\RGB card：RGB表，选色时的参考。 RGB table for reference.


设计思路及实现原理：

Design and Realization Principle

主引导程序（MBR）将保留扇区引导程序加载到内存后，由保留扇区引导程序进行设备、内存、处理器的初始化以及其他相关的初始化并提供可视化操作界面，最后加载存在于分区中的引导配置文件，由引导配置文件说明内核存在位置，最后将内核加载到指定内存。

After the loader in reserved sectors loaded to the memory by MBR, the initialization of devices, memory, processor and other hardwares will be done. And a visual operation interface will be provided. Finally, it will read configuration in the partition, which gives the location of the kernel on the hard disk, and load the kernel into memory.


需要的软件、工具：
Toolkits needed:

GCC

Nasm

一个十六进制编辑器 A Hex Editior

一个支持高亮的代码编辑器（可选，Windows下推荐Notepad++）  A code editior supporting highlight.

VirtualBox虚拟机（尽量使用旧的、稳定版本） VirtualBox (Old version is more stable)

