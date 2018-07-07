/**IDE控制器读扇区*/
int IDE_read(unsigned long storage_number, const void *dest, unsigned long LBA_addr, unsigned long n)
{
	printk("FFF6\n");
	short int *buffer = (short int *)dest;
	/**进行IDE通道判断*/
	if ((storage_number == SD_IDE_00) | (storage_number == SD_IDE_01))
	{
		/**若是第一通道的储存器*/
		printk("FFF5\n");
		/**循环读取扇区*/
		for (; n != 0; n --, buffer += (512 / sizeof(short int)), LBA_addr ++)
		{
			IDE_register[2] = 1;
			IDE_register[3] = (LBA_addr & 0xff);
			IDE_register[4] = ((LBA_addr >> 8) & 0xff);
			IDE_register[5] = ((LBA_addr >> 16) & 0xff);
			if (storage_number == SD_IDE_00)
			{
				printk("FFF4\n");
				/**若是主盘*/
				IDE_register[6] = (((LBA_addr >> 24) & 0xff) + 0b11100000);
			}else{
				/**若是从盘*/
				IDE_register[6] = (((LBA_addr >> 24) & 0xff) + 0b11110000);
			}
			IDE_register[7] = IDE_REG_CMD_READ;
			/**发出命令*/
			IDE_reg_cmd();
			printk("FFF4.2\n");
			/**等到IDE第一通道不忙时*/
			for (; IDE_busy(););
			printk("FFF4.3\n");
			/**读取数据*/
			unsigned long r;
			for (r = 0; r < (512 / sizeof(short int)); r ++)
			{
				buffer[r] = io_in16(IDE_reg_read_DATA);
			}
			printk("FFF3.2\n");
		}
	}else if ((storage_number == SD_IDE_10) | (storage_number == SD_IDE_11))
	{
		printk("FFF2\n");
		/**如果是第二通道的储存器*/
		warning(WARN_STORAGE_NOT_SUPPORT, "The IDE Secondary is not yet supported.");
		return STORAGE_RETVAL_ERR_NOT_SUPPORT;
	}else{
		printk("FFF1\n");
		/**若非IDE控制器的储存器*/
		return STORAGE_RETVAL_ERR_NOT_SUPPORT;
	}
	printk("FFF0\n");
	return STORAGE_RETVAL_NORMAL;
}

int IDE_read(unsigned long storage_number, const void *dest, unsigned long LBA_addr, unsigned long n)
{
	unsigned long offset, i;
	io_out16(IDE_reg_read_SECCOUNT, n);								/**数量*/
	io_out8(IDE_reg_read_LBA_0_7, (LBA_addr & 0xff));							/**LBA地址7~0*/
	io_out8(IDE_reg_read_LBA_8_15, ((LBA_addr >> 8) & 0xff));						/**LBA地址15~8*/
	io_out8(IDE_reg_read_LBA_16_23, ((LBA_addr >> 16) & 0xff));						/**LBA地址23~16*/
	io_out8(IDE_reg_read_LBA_D_24_27, (((LBA_addr >> 24) & 0xff) + 0xe0));			/**LBA地址27~24 + LBA模式，主硬盘*/
	io_out8(IDE_reg_read_STATUS, IDE_REG_CMD_READ);								/**读扇区*/
	/**循环从DATA端口读取数据*/
	for (i = 0; i != n; i ++)
	{
		hdd_wait();
		for (offset = 0; offset < 256; offset ++)
		{
			dest[(i * 256) + offset] = io_in16(IDE_reg_read_DATA);		/**从DATA端口中读取数据*/
		}
	}
	return;
}