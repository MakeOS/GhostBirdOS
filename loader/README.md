Copyright 2018 GhostBird OS

Lab Explorer Loader
(本引导程序所使用的许可证类型尚不明朗。)
(The lisence still remains uncertain.)

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
(Old Use) .bat文件：Win32下的批处理文件 Win32 Batch Files

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

开发探索者内核引导程序所需要的软件、工具：
Toolkits needed:
GCC
Nasm
一个十六进制编辑器 A Hex Editior
一个支持高亮的代码编辑器（可选，Windows下推荐Notepad++）  A code editior supporting highlight.
dd 或其他镜像操作程序 dd or other compatible image operating programs
VirtualBox虚拟机（尽量使用旧的、稳定版本） VirtualBox (Old version is more stable)
测试目前只是在VBox中进行，还未对其他虚拟机中测试，无法保证在其他虚拟机里面能够运行。
We have currently tested in VBox, but in other kinds of virtual machines. Working in other VMs is not proved.

在虚拟机中使用本引导程序的相关步骤：
Steps to use the loader in VM:
1.准备好上述所需的软件，编译或获取正常的MBR.bin以及BOOT文件。
  Prepare the tools above to get MBR.bin and BOOT(file).
2.在VirtualBox中新建一个名称为“Explorer”、类型为Other、版本为Other/Unknown的虚拟机，内存分配4MB以上，创建一个VHD形式、非动态分配的虚拟磁盘（至少4MB，实验室开发时已确保64MB或以上大小的虚拟磁盘可以正常使用）作为IDE接口磁盘，将虚拟机的显存设置为最少9MB，最好开启VT-x/AMD-V技术加速，其他设置无要求。
  Create a VM, typed "Other", with a version of "Other/Unknown". The memory allocated should be greater than 4MB. And create a VHD format, static allocated virtual hard disk (4MB at least, 64MB at least when developing), through IDE Channel. The display memory is 4MB at least. We recommened to enable virtualization if possible.
3.打开任意一款支持编辑VHD虚拟磁盘的分区软件，或打开Windows的磁盘管理（Windows Vista以上：控制面板〉系统和安全〉创建并格式化硬盘分区），在菜单栏的动作〉附加VHD中，找到刚才生成的虚拟磁盘，将其挂载，进行MBR初始化并至少分出一个FAT32分区，并将其标记为活动分区。再将其脱离。
  Write MBR. Partition at least one FAT32 partition, and select it as a active partition.
4.打开HxD并向其中加入MBR、BOOT、刚才创建的VHD虚
  拟磁盘，将MBR的446字节覆盖拷贝到VHD虚拟磁盘的
  起初0x00000000~0x0x000001BD之中，MBR安装完成。
5.将最大不超过63.5KB（可能不是整数）的BOOT的所有
  字节覆盖拷贝到VHD虚拟磁盘起初0x00000200开始处
6.启动虚拟机。
若在开发过程中仅有MBR和BOOT中的一项存在更新，可
以在上述1~6步骤完成后仅拷贝相应十六进制数据进入
VHD虚拟磁盘中作相应调试。
  
开发者提示：
1.除非特殊情况，开发者需要时刻保持优秀的代码编
写以及项目维护的习惯，尽量不使用容易造成误解的
表达方法。
2.创建新文件（Makefile、源代码文件）时，受制于
本级Makefile下的清理生成文件方法，绝对禁止源代
码的文件名为BOOT、MBR或任意.O文件、.bin文件。
为了兼容以及更加显眼，建议Makefile首字母大写而
不是makefile或GUNmakefile，创建目录时需要同时创
建该目录下的Makefile和README（强烈建议），在文
件的头部应当添加与该引导程序中其他同类文件相似
或者更加详细、清晰的文件描述。
3.若在boot部分加入C或ASM源代码，需要在相应层级
的Makefile生成规则中添加相应编译规则，并有可能
需要在boot层级中的Makefile内的ld语句中添加新的
链接文件。
4.涉及到文件路径处建议使用“/”（Win32下）。

Win32和Linux平台开发的兼容性问题：
本引导程序项目允许在Win32、Linux任一平台上开发，
由于上述两个平台的开发环境可能存在差异，所以本
引导程序项目在上述两个平台上开发切换时需要进行
一些改动，其中包括但不限于：
1.console.bat在Linux平台上的不可用
2.本级Makefile中启动虚拟机部分语句的修改
3.本级Makefile中del和rm的相互转换
4.Makefile、.C文件、.h文件、.asm文件中与文件路
径有关的相关表述（“\”符号仅适用于Win32，“/”符号
同时适用于Win32和Linux）
5.代码编辑器的重新选择
6.Win32下的Winimage在Linux下的替代选择dd命令
7.十六进制编辑器的重新选择
