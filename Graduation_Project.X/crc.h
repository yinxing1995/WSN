#ifndef _CRC_H
#define _CRC_H
#include <xc.h> // include processor files - each processor file is guarded.


unsigned char CalculateCRC(unsigned char* message, unsigned char length);

#endif
