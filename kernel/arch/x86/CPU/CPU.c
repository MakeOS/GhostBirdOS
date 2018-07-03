/**
 * Copyright 2013-2015 by Explorer Developers.
 * made by Rose<roselyu@163.com> Lab Explorer Developers<1@GhostBirdOS.org>
 * Explorer Initialization in the start of kernel
 * Explorer 0.01/arch/x86/CPU/CPU.c
 * version:Alpha
 * 9/14/2014 12:30 PM:created
 * 1/3/2015 3:24 PM:rename CPU.c & remove to arch/x86/CPU/
 */

#include "CPU.h"
#include <stdbool.h>

bool support_CPUID;
unsigned int CPU_MAXID;
unsigned int CPU_EMAXID;
unsigned char CPU_OEM[13];
unsigned int CPUID_model;
unsigned int display_family;
unsigned int display_model;
unsigned char *CPU_family;
unsigned char *CPU_model;

void init_CPU(void)
{
	/**
	 *We can make sure this CPU is Intel 80386,
	 *If this CPU no support CPUID instruction.
	 *else this CPU is Intel 80486 or after.
	 *The method of check CPU if it can support CPUID
	 *is save Register EFLAGS and try to change the bit 21
	 *in Register EFLAGS, than compare if we write successful.
	 */
	

/*****************************WARING:can't add printk here*****************************/
/*****************************PLEASE ADD PRINTK TO output_CPU_info*****************************/
	/**
	 *get CPU OEM & CPUID MAX support
	 *CPUID.00H:EBX:EDX:ECX is string of OEM.
	 *CPUID.00H:EAX is MAX number basic CPUID support from CPU.
	 */
	CPU_OEM[12] = 0x00;
	asm volatile(" \
		xor		%eax,%eax; \
		cpuid; \
		movl	%eax,(CPU_MAXID); \
		movl	%ebx,(CPU_OEM); \
		movl	%edx,(CPU_OEM+4); \
		movl	%ecx,(CPU_OEM+8); \
	");
	
	/**
	 *get extended MAX value of CPUID instruction
	 */
	asm volatile(" \
		movl	$0x80000000,%eax; \
		cpuid; \
		movl	%eax,(CPU_EMAXID); \
	");
	
	/**
	 *get family & model
	 */


	
	if (strncmp(CPU_OEM, "GenuineIntel", 12) == 0)
	{
		asm volatile(" \
			movl	$0x01,%eax; \
			cpuid; \
			movl	%eax,(CPUID_model); \
		");
		if (FAMILY == 0x0f)
		{
			display_family = EXTENDED_FAMILY + FAMILY;
		}else{
			display_family = FAMILY;
		}
		
		if (FAMILY == 0x6 || FAMILY == 0xf)
		{
			display_model = (EXTENDED_MODEL << 4) + MODEL;
		}else{
			display_model = MODEL;
		}
		
		/**
		 *CPU family & model information
		 */
		if (display_family == 0x6)
		{
			CPU_family = CPU_family_06H;
			if (display_model == 0x2a)
			{
				CPU_model = CPU_model_06_2AH;
			}else if (display_model == 0xf)
			{
				/**
				 *This place short-write CPU model because there are to many CPU
				 *use this family and model,done this if you need support another CPU in
				 *this family and model,you can visited page 3091 in
				 *<Intel 64 and IA-32 architectures software developer's Manual>
				 */
				CPU_model = CPU_model_06_0FH;
			}
			
		}else if (display_family == 0xf)
		{
			CPU_family = CPU_family_0FH;
		}else{
			CPU_family = CPU_family_Unknown;
		}
	}else if (strncmp(CPU_OEM, "AuthenticAMD", 12) == 0)
	{
		CPU_family = CPU_family_AMD;
		CPU_model = CPU_model_Unknown;
	}else if (strncmp(CPU_OEM, "AMDisbetter!", 12) == 0)
	{
		CPU_family = CPU_family_AMDK5;
		CPU_model = CPU_model_Unknown;
	}else if (strncmp(CPU_OEM, "SiS SiS SiS ", 12) == 0)
	{
		CPU_family = CPU_family_SiS;
		CPU_model = CPU_model_Unknown;
	}else if (strncmp(CPU_OEM, "VIA VIA VIA ", 12) == 0)
	{
		CPU_family = CPU_family_VIA;
		CPU_model = CPU_model_Unknown;
	}else if (strncmp(CPU_OEM, "Microsoft Hv", 12) == 0)
	{
		CPU_family = CPU_family_vpc;
		CPU_model = CPU_model_Unknown;
	}else if (strncmp(CPU_OEM, "VMwareVMware", 12) == 0)
	{
		CPU_family = CPU_family_VMware;
		CPU_model = CPU_model_Unknown;
	}else
	{
		CPU_family = CPU_family_Unknown;
		CPU_model = CPU_model_Unknown;	
	}
}

void output_CPU_info(void)
{
	unsigned int brands[4]; //每次的eax、ebx、ecx、edx
	unsigned int i;

	/**空出一行*/
	printk("\n");
	
	printk("CPU information:\n");
	printk("CPU manufacturer information:%s\n", &CPU_OEM);
	printk("CPU model:");
        for (i = 0x80000002; i <= 0x80000004; i++) {
            asm volatile ("cpuid"
                      : "=a"(brands[0]), "=b"(brands[1]), "=c"(brands[2]), "=d"(brands[3])
                      : "0" (i)
                      );
            printk("%s", (char *)brands);
        }
	printk("\nCPUID support in MAX:0x%X,extended CPUID support in MAX:0x%X.\n", CPU_MAXID, CPU_EMAXID);
	printk("CPU family:%s[0x%X],model:%s[0x%X].\n", CPU_family,display_family, CPU_model, display_model);
}