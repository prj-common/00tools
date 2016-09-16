#include <stdio.h>
#include <stdlib.h>
#include "app_common.h"

#define VERSION_LIBTOOLS   "0.0.1"

int app_libtools_help(void)
{
	printf("Usage: libtools <Function> [Other Parameter]\n");
	printf("Function parameter list: \n");
	printf("-version  Show version number and quit\n");
	printf("-crc\n");
	printf("    -crc16              <string>   Calculate crc-16\n");
	printf("    -crc16_modbus       <string>   Calculate crc-16(Modbus)\n");
	printf("    -crc16_sick         <string>   Calculate crc-16(Sick)\n");
	printf("    -crc16_ccitt_xmodem <string>   Calculate crc-ccitt(Xmodem)\n");
	printf("    -crc16_ccitt_0xffff <string>   Calculate crc-ccitt(0xffff)\n");
	printf("    -crc16_ccitt_kermit <string>   Calculate crc-ccitt(Kermit)\n");
	printf("    -crc16_ccitt_dnp    <string>   Calculate crc-ccitt(Dnp)\n");
	printf("    -crc32              <string>   Calculate crc-32\n");
	printf("    -all                <string>   Calculate all crc value at above list\n");
	printf("-md5\n");
	printf("    -f    <file name>\n");
	printf("    -a    <ascii string>\n");
	return 0;
}

int app_libtools_print_help_prompt(void)
{
	printf("libtools: try \'libtools -help\' for more information\n");
	return 0;
}

int app_libtools_show_version(void)
{
	printf("libtools %s\n", VERSION_LIBTOOLS);
	return 0;
}
