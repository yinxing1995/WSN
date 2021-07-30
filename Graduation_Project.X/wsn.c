#include <xc.h>
#include "wsn.h"
#include "delays.h"
#include "crc.h"
#include "MRF24J40.h"

#define WRITE_TXNMTRIG (0x37)

extern void MRF24J40Init(short int panid, short int shortaddress);
extern void SetLongRAMAddr(unsigned int address, unsigned char value);
extern void SetShortRAMAddr(unsigned char address, unsigned char value);
extern unsigned char PHYGetRSSI(void);



void WSNTransmit(char *Data, unsigned char NoBytes,short int panid, short int dest_address, short int sour_address)
{
    do
    {
        Delay1KTCYx(5+rand()%10);
    }while(PHYGetRSSI());
    unsigned char i;
	SetLongRAMAddr(0,NoBytes+9);
	SetLongRAMAddr(1,NoBytes+9);	//length of data in bytes		
	SetLongRAMAddr(2,0x01);			//FCF set for short addressing, no ack request
	SetLongRAMAddr(3,0x88);
	SetLongRAMAddr(4,0x01);			//sequence number set to 1
	SetLongRAMAddr(5,(unsigned char)(panid%16));			//PAN ID set to broadcast
	SetLongRAMAddr(6,(unsigned char)(panid/16));
	SetLongRAMAddr(7,(unsigned char)(dest_address%16));			//destination address set to broadcast
	SetLongRAMAddr(8,(unsigned char)(dest_address/16));
	SetLongRAMAddr(9,(unsigned char)(sour_address%16));			//source address set to broadcast
	SetLongRAMAddr(10,(unsigned char)(sour_address/16));
	for(i=0; i<NoBytes; i++) SetLongRAMAddr(11+i,Data[i]);	//write data to Tx FIFO
	SetShortRAMAddr(WRITE_TXNMTRIG,0b00000001);		//trigger Tx FIFO
}

void Node_Init()
{
    switch(NODE_ID)
    {
        case 1:
            MRF24J40Init(PAN_ID, SHORT_ADDR_1);
            source_addr = SHORT_ADDR_1;
            destination_addr = SHORT_ADDR_1;
            break;
        case 2:
            MRF24J40Init(PAN_ID, SHORT_ADDR_2);
            source_addr = SHORT_ADDR_2;
            destination_addr = SHORT_ADDR_1;
            break;
        case 3:
            MRF24J40Init(PAN_ID, SHORT_ADDR_3);
            source_addr = SHORT_ADDR_3;
            destination_addr = SHORT_ADDR_1;
            break;
        case 4:
            MRF24J40Init(PAN_ID, SHORT_ADDR_4);
            source_addr = SHORT_ADDR_4;
            destination_addr = SHORT_ADDR_1;
            break;
        default:
            return;
    }
    SetChannel(CHANNEL);
}

unsigned char WSNRecieve(char *Data)
{
    int Strength = 0;
    return PHYReceive(Data,&Strength);
}


unsigned char *MessageGet()
{
    if(!WSNRecieve(RXPacket))
        return NULL;
    else
    {
        return &RXPacket;
    }
}
