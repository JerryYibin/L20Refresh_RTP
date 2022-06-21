/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "QuickCRC32Algorithm.h"

QuickCRC32Algorithm::QuickCRC32Algorithm() {
	// TODO Auto-generated constructor stub

}

QuickCRC32Algorithm::~QuickCRC32Algorithm() {
	// TODO Auto-generated destructor stub
}

unsigned int QuickCRC32Algorithm::crc32_ccitt(const void *buf, const unsigned int len)
{
	int counter = 0;
    unsigned int crc32 = 0xFFFFFFFF;
    for (counter = 0; counter < len; counter++)
    {
        crc32 = (crc32 << CRC_8BIT_SHIFT)
            ^ crc32_table[((crc32 >> CRC_24BIT_SHIFT) ^ *(unsigned char *)buf) & 0x00FF];
        buf = ((char *) buf) + 1;
    }
    return crc32;
}
