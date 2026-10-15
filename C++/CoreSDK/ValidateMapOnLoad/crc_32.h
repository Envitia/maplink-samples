#ifndef _CRC_32_H_
#define _CRC_32_H_
/* Copyright (C) 1986 Gary S. Brown.  You may use this program, or
   code or tables extracted from it, as desired without restriction.*/

/* Need an unsigned type capable of holding 32 bits; */
typedef unsigned int UNS_32_BITS;

/* Function to create CRC32 from a buffer */
extern UNS_32_BITS crc32buf(const char *buf, size_t len);

/* Function to create CRC32 from a files contents */
extern bool crc32file(FILE *fin, UNS_32_BITS &crc, size_t &charcnt);

/* Function to create CRC32 from a files contents - potentially ignoring some of the end of the file*/
extern bool crc32fileSpecificBytesOnly(FILE *fin, size_t bytesToCheck, UNS_32_BITS &crc);


#endif
