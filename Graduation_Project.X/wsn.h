 
#ifndef _WSN_H
#define	_WSN_H

#include <xc.h> // include processor files - each processor file is guarded.


#define PAN_ID 0x1212
#define SHORT_ADDR_1 0x1111
#define SHORT_ADDR_2 0x2222
#define SHORT_ADDR_3 0x3333
#define SHORT_ADDR_4 0x4444
#define CHANNEL 12

#define NODE_ID 3
#define REPORT_TIMER 3
//#define TIMES_ROUTING 3 //4 nodes

char Report_time;
char RXPacket[128];

unsigned short int destination_addr, source_addr;
unsigned int Strength, CRCRight;


void WSNTransmit(char *Data, unsigned char NoBytes, short int panid, short int dest_address, short int sour_address);

void Node_Init(void);

unsigned char WSNRecieve(char *Data);

unsigned char *MessageGet(void);


#endif	/* XC_HEADER_TEMPLATE_H */

