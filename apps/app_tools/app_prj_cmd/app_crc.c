#include <stdio.h>
#include <stdlib.h>
#include "app_common.h"

int app_crc16_main(int argc, char *argv[])
{
	// printf("app crc16 main\n");
	if(argc < 4)
	{
		printf("cmd error!\n");
		return 0;
	}

	if(argc == 4) // default handler
	{
		unsigned short crc16 = 0;
		crc16 = bl_crc16(argv[3], bl_strlen(argv[3]), 0);
		printf("string: %s\n", argv[3]);
		printf("CRC16: 0x%X [%d]\n", crc16, crc16);
		return 0;
	}

	if(!bl_strcmp(argv[3], "-a"))
	{
		unsigned short crc16 = 0;
		crc16 = bl_crc16(argv[4], bl_strlen(argv[4]), 0);
		printf("string: %s\n", argv[4]);
		printf("CRC16: 0x%X [%d]\n", crc16, crc16);
	}
	else
	{
		printf("parameter error\n");
	}
}

int app_crc16_modbus_main(int argc, char *argv[])
{
	// printf("app crc16 main\n");
	if(argc < 4)
	{
		printf("cmd error!\n");
		return 0;
	}

	if(argc == 4) // default handler
	{
		unsigned short crc16 = 0;
		crc16 = bl_crc16(argv[3], bl_strlen(argv[3]), 0xffff);
		printf("string: %s\n", argv[3]);
		printf("CRC16(MODBUS): 0x%X [%d]\n", crc16, crc16);
		return 0;
	}

	if(!bl_strcmp(argv[3], "-a"))
	{
		unsigned short crc16 = 0;
		crc16 = bl_crc16(argv[4], bl_strlen(argv[4]), 0xffff);
		printf("string: %s\n", argv[4]);
		printf("CRC16(MODBUS): 0x%X [%d]\n", crc16, crc16);
	}
	else
	{
		printf("parameter error\n");
	}
}

int app_crc16_sick_main(int argc, char *argv[])
{
	// printf("app crc16 main\n");
	if(argc < 4)
	{
		printf("cmd error!\n");
		return 0;
	}

	if(argc == 4) // default handler
	{
		unsigned short crc16 = 0;
		crc16 = bl_crc16_sick(argv[3], bl_strlen(argv[3]), 0);
		printf("string: %s\n", argv[3]);
		printf("CRC16(SICK): 0x%X [%d]\n", crc16, crc16);
		return 0;
	}

	if(!bl_strcmp(argv[3], "-a"))
	{
		unsigned short crc16 = 0;
		crc16 = bl_crc16_sick(argv[4], bl_strlen(argv[4]), 0);
		printf("string: %s\n", argv[4]);
		printf("CRC16(SICK): 0x%X [%d]\n", crc16, crc16);
	}
	else
	{
		printf("parameter error\n");
	}
}

int app_crc16_ccitt_xmodem_main(int argc, char *argv[])
{
	// printf("app crc16 main\n");
	if(argc < 4)
	{
		printf("cmd error!\n");
		return 0;
	}

	if(argc == 4) // default handler
	{
		unsigned short crc16 = 0;
		crc16 = bl_crc16_ccitt(argv[3], bl_strlen(argv[3]), 0);
		printf("string: %s\n", argv[3]);
		printf("CRC-CCITT(XMODEM): 0x%X [%d]\n", crc16, crc16);
		return 0;
	}

	if(!bl_strcmp(argv[3], "-a"))
	{
		unsigned short crc16 = 0;
		crc16 = bl_crc16_ccitt(argv[4], bl_strlen(argv[4]), 0);
		printf("string: %s\n", argv[4]);
		printf("CRC-CCITT(XMODEM): 0x%X [%d]\n", crc16, crc16);
	}
	else
	{
		printf("parameter error\n");
	}
}

int app_crc16_ccitt_0xffff_main(int argc, char *argv[])
{
	// printf("app crc16 main\n");
	if(argc < 4)
	{
		printf("cmd error!\n");
		return 0;
	}

	if(argc == 4) // default handler
	{
		unsigned short crc16 = 0;
		crc16 = bl_crc16_ccitt(argv[3], bl_strlen(argv[3]), 0xffff);
		printf("string: %s\n", argv[3]);
		printf("CRC-CCITT(0XFFFF): 0x%X [%d]\n", crc16, crc16);
		return 0;
	}

	if(!bl_strcmp(argv[3], "-a"))
	{
		unsigned short crc16 = 0;
		crc16 = bl_crc16_ccitt(argv[4], bl_strlen(argv[4]), 0xffff);
		printf("string: %s\n", argv[4]);
		printf("CRC-CCITT(0XFFFF): 0x%X [%d]\n", crc16, crc16);
	}
	else
	{
		printf("parameter error\n");
	}
}

int app_crc16_ccitt_0x1d0f_main(int argc, char *argv[])
{
	// printf("app crc16 main\n");
	if(argc < 4)
	{
		printf("cmd error!\n");
		return 0;
	}

	if(argc == 4) // default handler
	{
		unsigned short crc16 = 0;
		crc16 = bl_crc16_ccitt(argv[3], bl_strlen(argv[3]), 0x1d0f);
		printf("string: %s\n", argv[3]);
		printf("CRC-CCITT(0X1D0F): 0x%X [%d]\n", crc16, crc16);
		return 0;
	}

	if(!bl_strcmp(argv[3], "-a"))
	{
		unsigned short crc16 = 0;
		crc16 = bl_crc16_ccitt(argv[4], bl_strlen(argv[4]), 0x1d0f);
		printf("string: %s\n", argv[4]);
		printf("CRC-CCITT(0X1D0F): 0x%X [%d]\n", crc16, crc16);
	}
	else
	{
		printf("parameter error\n");
	}
}

// kermit 0x8408
int app_crc16_ccitt_kermit_main(int argc, char *argv[])
{
	// printf("app crc16 main\n");
	if(argc < 4)
	{
		printf("cmd error!\n");
		return 0;
	}

	if(argc == 4) // default handler
	{
		unsigned short crc16 = 0;
		crc16 = bl_crc16_ccitt_kermit(argv[3], bl_strlen(argv[3]), 0);
		printf("string: %s\n", argv[3]);
		printf("CRC-CCITT(KERMIT): 0x%X [%d]\n", crc16, crc16);
		return 0;
	}

	if(!bl_strcmp(argv[3], "-a"))
	{
		unsigned short crc16 = 0;
		crc16 = bl_crc16_ccitt_kermit(argv[4], bl_strlen(argv[4]), 0);
		printf("string: %s\n", argv[4]);
		printf("CRC-CCITT(KERMIT): 0x%X [%d]\n", crc16, crc16);
	}
	else
	{
		printf("parameter error\n");
	}
}

int app_crc16_dnp_main(int argc, char *argv[])
{
	// printf("app crc16 main\n");
	if(argc < 4)
	{
		printf("cmd error!\n");
		return 0;
	}

	if(argc == 4) // default handler
	{
		unsigned short crc16 = 0;
		crc16 = bl_crc16_dnp(argv[3], bl_strlen(argv[3]), 0);
		printf("string: %s\n", argv[3]);
		printf("CRC-16(DNP): 0x%X [%d]\n", crc16, crc16);
		return 0;
	}

	if(!bl_strcmp(argv[3], "-a"))
	{
		unsigned short crc16 = 0;
		crc16 = bl_crc16_dnp(argv[4], bl_strlen(argv[4]), 0);
		printf("string: %s\n", argv[4]);
		printf("CRC-16(DNP): 0x%X [%d]\n", crc16, crc16);
	}
	else
	{
		printf("parameter error\n");
	}
}

int app_crc32_main(int argc, char *argv[])
{
	// printf("app crc16 main\n");
	if(argc < 4)
	{
		printf("cmd error!\n");
		return 0;
	}

	if(argc == 4) // default handler
	{
		unsigned long crc32 = 0;
		crc32 = bl_crc32(argv[3], bl_strlen(argv[3]), 0xffffffffL);
		printf("string: %s\n", argv[3]);
		printf("CRC-32: 0x%X [%d]\n", crc32, crc32);
		return 0;
	}

	if(!bl_strcmp(argv[3], "-a"))
	{
		unsigned short crc32 = 0;
		crc32 = bl_crc32(argv[4], bl_strlen(argv[4]), 0xffffffffL);
		printf("string: %s\n", argv[4]);
		printf("CRC-32: 0x%X [%d]\n", crc32, crc32);
	}
	else
	{
		printf("parameter error\n");
	}
}

int app_crc_all_main(int argc, char *argv[]) // todo
{
	printf("todo\n");

	return 0;
}

int app_crc_main(int argc, char *argv[])
{
	// printf("app crc main\n");
	if(argc < 3)
	{
		printf("parameter error\n");
		return 0;
	}

	if(!bl_strcmp(argv[2], "-all"))
	{
		app_crc_all_main(argc, argv);
	}
	else if(!bl_strcmp(argv[2], "-crc16"))
	{
		app_crc16_main(argc, argv);
	}
	else if(!bl_strcmp(argv[2], "-crc16_modbus"))
	{
		app_crc16_modbus_main(argc, argv);
	}	
	else if(!bl_strcmp(argv[2], "-crc16_sick"))
	{
		app_crc16_sick_main(argc, argv);
	}
	else if(!bl_strcmp(argv[2], "-crc16_ccitt_xmodem"))
	{
		app_crc16_ccitt_xmodem_main(argc, argv);
	}
	else if(!bl_strcmp(argv[2], "-crc16_ccitt_0xffff"))
	{
		app_crc16_ccitt_0xffff_main(argc, argv);
	}	
	else if(!bl_strcmp(argv[2], "-crc16_ccitt_0x1d0f"))
	{
		app_crc16_ccitt_0x1d0f_main(argc, argv);
	}		
	else if(!bl_strcmp(argv[2], "-crc16_ccitt_kermit"))
	{
		app_crc16_ccitt_kermit_main(argc, argv);
	}
	else if(!bl_strcmp(argv[2], "-crc16_dnp"))
	{
		app_crc16_dnp_main(argc, argv);
	}	
	else if(!bl_strcmp(argv[2], "-crc32"))
	{
		app_crc32_main(argc, argv);
	}
	else
	{
		printf("parameter error\n");
	}
	
	return 0;
}

