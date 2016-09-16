#include "bl_crc.h"

static unsigned short
crc16_256tab_update(unsigned char data, unsigned short crc)
{
	unsigned short int table[256] = {
		0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
		0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
		0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
		0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
		0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
		0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
		0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
		0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
		0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
		0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
		0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
		0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
		0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
		0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
		0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
		0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
		0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
		0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
		0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
		0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
		0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
		0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
		0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
		0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
		0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
		0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
		0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
		0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
		0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
		0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
		0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
		0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 
	};
	unsigned char tmp;

	tmp = data^crc;
	crc >>= 8;
	crc ^= table[tmp];
	return crc;
}

static unsigned short int 
crc16_16tab_update(unsigned char data, unsigned short crc)
{
	unsigned short int table[16] = {
	0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
	0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
	};
	unsigned short int r;

	/* compute checksum of lower four bits of *nData */
	r = table[crc & 0xF];
	crc = (crc >> 4) & 0x0FFF;
	crc = crc ^ r ^ table[data & 0xF];

	/* now compute checksum of upper four bits of *nData */
	r = table[crc & 0xF];
	crc = (crc >> 4) & 0x0FFF;
	crc = crc ^ r ^ table[(data >> 4) & 0xF];

	return(crc);
}

static unsigned short 
crc16_ccitt_256tab_update(unsigned char data,  unsigned short crc)    
{   
    unsigned char da;
	const unsigned short crc_ta[256] = {
		0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,   
		0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,   
		0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,   
		0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,   
		0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,   
		0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,   
		0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,   
		0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,   
		0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,   
		0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,   
		0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,   
		0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,   
		0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,   
		0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,   
		0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,   
		0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,   
		0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,   
		0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,   
		0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,   
		0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,   
		0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,   
		0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,   
		0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,   
		0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,   
		0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,   
		0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,   
		0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,   
		0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,   
		0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,   
		0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,   
		0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,   
		0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0   
	};   

	da=(unsigned char) (crc/256);
	crc<<=8;
	crc^=crc_ta[da^data];
	return(crc);   
}   

static unsigned short 
crc16_ccitt_16tab_update(unsigned char data, unsigned short crc)
{
	unsigned char tmp;
	static const unsigned short table[]=
	{
		0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
		0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
	};

	tmp = (unsigned char)(crc>>8);
	tmp ^= data;
	tmp >>= 4;
	crc <<= 4;
	crc ^= table[tmp];

	tmp = (unsigned char)(crc>>8);
	tmp >>=4;
	tmp ^= data;
	tmp &= 0x0f;
	crc <<= 4;
	crc ^= table[tmp];

	return crc;
}

static unsigned short 
crc16_sick_update(unsigned char c, unsigned short crc, unsigned char pre_byte) 
{
    unsigned short short_c, short_p;

    short_c  =   0x00ff & c;
    short_p  = ( 0x00ff & (unsigned short) pre_byte ) << 8;

    if (crc & 0x8000) 
	{
		crc = ( crc << 1)^0x8005;
	}
    else 
	{
		crc = crc << 1;
	}
    crc &= 0xffff;
    crc ^= ( short_c | short_p );

    return crc;
}

unsigned short 
bl_crc16(unsigned char* data,unsigned short len, unsigned short start)
{
    unsigned short crc = start;

    while(len)
    {
        len--;
#if 1
        crc = crc16_16tab_update(*data++, crc);
#else
		crc = crc16_256tab_update(*data++, crc);
#endif
    }

	return crc;
}

unsigned short 
bl_crc16_ccitt(unsigned char* data,unsigned short len, unsigned short start)
{
    unsigned short crc = start;

    while(len)
    {
        len--;
#if 1
        crc = crc16_ccitt_16tab_update(*data++, crc);
#else
		crc = crc16_ccitt_256tab_update(*data++, crc);
#endif
    }

	return crc;
}

unsigned short 
bl_crc16_sick(unsigned char* data,unsigned short len, unsigned short start)
{
    unsigned short crc = start;
	unsigned char pre_byte = 0;

    while(len)
    {
        len--;
		crc = crc16_sick_update(*data, crc, pre_byte);
		pre_byte = *data++;
    }

	/* exchange low byte and high byte */
	return (crc<<8)|(crc>>8);
}

// *************************************************
// dnp, kermit, crc32

unsigned short 
bl_crc16_ccitt_kermit(unsigned char* data,unsigned short len, unsigned short start)
{
	unsigned short crc = start;
	unsigned char pre_byte = 0;

	while(len)
	{
		len--;
		crc = update_crc_kermit(crc, *data);
		pre_byte = *data++;
	}

	/* exchange low byte and high byte */
	return (crc<<8)|(crc>>8);
}

unsigned short
bl_crc16_dnp(unsigned char* data,unsigned short len, unsigned short start)
{
	unsigned short crc = start;
	unsigned char pre_byte = 0;

	while(len)
	{
		len--;
		crc = update_crc_dnp(crc, *data);
		pre_byte = *data++;
	}
	crc = ~crc;

	/* exchange low byte and high byte */
	return (crc<<8)|(crc>>8);
}

unsigned long
bl_crc32(unsigned char* data,unsigned short len, unsigned long start)
{
	unsigned long crc = start;
	unsigned char pre_byte = 0;

	while(len)
	{
		len--;
		crc = update_crc_32(crc, *data);
		pre_byte = *data++;
	}

	crc ^= 0xffffffffL;
	return crc;
}

/*
http://www.lammertbies.nl/comm/info/crc-calculation.html
*/
void bl_crc_test(void)
{
	char test[16];
	char test_len = 0;
	unsigned short test_crc_16 = 0;

	// test data: "123456789"
	// test data len: 9
	memset(test, 0, 16);
	test_len = 9;
	memcpy(test, "123456789", 9);
	printf("string: %s\n", test);

	test_crc_16 = bl_crc16(test, test_len, 0);
	printf("crc16: 0x%04X\n", test_crc_16);
	
	test_crc_16 = bl_crc16(test, test_len, 0xffff);
	printf("crc16(Modbus): 0x%04X\n", test_crc_16);

	test_crc_16 = bl_crc16_sick(test, test_len, 0);
	printf("crc16(Sick): 0x%04X\n", test_crc_16);

	test_crc_16 = bl_crc16_ccitt(test, test_len, 0);
	printf("crc-ccitt(XModem): 0x%04X\n", test_crc_16);
	
	test_crc_16 = bl_crc16_ccitt(test, test_len, 0xffff);
	printf("crc-ccitt(0xFFFF): 0x%04X\n", test_crc_16);

	test_crc_16 = bl_crc16_ccitt(test, test_len, 0x1d0f);
	printf("crc-ccitt(0x1D0F): 0x%04X\n", test_crc_16);

	//kermit 0x8408
	// todo
}

