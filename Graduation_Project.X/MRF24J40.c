#include "MRF24J40.h"
#include <stdlib.h>                              
#include <stdio.h>
#include <spi.h>                       
#include <string.h>                            
#include <timers.h>
#include "MRF24J40.h"                      
#include <p18c452.h>

//*********************************************************************
//* void PHYSetLongRAMAddr(unsigned int address, unsigned char value)
//* writes to register with long address
//*********************************************************************

void SetLongRAMAddr(unsigned int address, unsigned char value)
{
    volatile unsigned char tmpINT0IE = INTCONbits.INT0IE;
    INTCONbits.INT0IE = 0;
    PORTCbits.RC0 = 0;
    WriteSPI((((unsigned char)(address>>3))&0b01111111)|0x80);
    WriteSPI((((unsigned char)(address<<5))&0b11100000)|0x10);
    WriteSPI(value);
    PORTCbits.RC0 = 1;
    INTCONbits.INT0IE = tmpINT0IE;
}

//*********************************************************************
//* void PHYSetShortRAMAddr(unsigned int address, unsigned char value)
//* writes to register with short address
//*********************************************************************

void SetShortRAMAddr(unsigned char address, unsigned char value)
{
    volatile unsigned char tmpINT0IE = INTCONbits.INT0IE;
    INTCONbits.INT0IE = 0;
    PORTCbits.RC0 = 0;       
    WriteSPI(address);
    WriteSPI(value);
    PORTCbits.RC0 = 1;
    INTCONbits.INT0IE = tmpINT0IE;
}
//*********************************************************************
//* void PHYGetShortRAMAddr(unsigned int address, unsigned char value)
//* reads from register with short address
//*********************************************************************

unsigned char GetShortRAMAddr(unsigned char address)
{
    unsigned char toReturn;
    volatile unsigned char tmpINT0IE = INTCONbits.INT0IE;
    INTCONbits.INT0IE = 0;
    PORTCbits.RC0 = 0;
    WriteSPI(address);
    toReturn = ReadSPI();
    PORTCbits.RC0 = 1;
    INTCONbits.INT0IE = tmpINT0IE;
    return toReturn;
}


//*********************************************************************
//* void PHYGetLongRAMAddr(unsigned int address, unsigned char value)
//* reads from register with long address
//*********************************************************************

unsigned char GetLongRAMAddr(unsigned int address)
{
    unsigned char toReturn;
    volatile unsigned char tmpINT0IE = INTCONbits.INT0IE;
    INTCONbits.INT0IE = 0;
    PORTCbits.RC0 = 0;
    WriteSPI(((address>>3)&0b01111111)|0x80);
    WriteSPI(((address<<5)&0b11100000));
    toReturn = ReadSPI();
    PORTCbits.RC0 = 1;
    INTCONbits.INT0IE = tmpINT0IE;
    return toReturn;
}


//*********************************************************************
//** void SetChannel(unsigned char channel)
//** sets radio channel 
//********************************************************************

void SetChannel(unsigned char channel)
{
    SetLongRAMAddr(RFCTRL0, (channel | 0x02));
    SetShortRAMAddr(WRITE_RFCTL,0x04);
    SetShortRAMAddr(WRITE_RFCTL,0x00);
}

//*********************************************************************
//**** 	MRF24J40Init (initialises transceiver) ************************
//*********************************************************************

void MRF24J40Init(short int panid, short int shortaddress)
{  
    unsigned char i;
    unsigned int j;
    
    PORTCbits.RC2 = 0;			//reset transceiver
    for(j=0;j<300;j++);
    PORTCbits.RC2 = 1;
    for(j=0;j<300;j++);
  
	SetShortRAMAddr(WRITE_RFCTL,0x04);		//reset the RF module 
	SetShortRAMAddr(WRITE_RFCTL,0x00);		//remove the RF module from reset
	SetShortRAMAddr(WRITE_RXFLUSH,0x05);	//receive only data packets & flush the RX fifo 	
	SetShortRAMAddr(WRITE_SADRL,(unsigned char)(shortaddress%16));	//Program the short MAC Address
	SetShortRAMAddr(WRITE_SADRH,(unsigned char)(shortaddress/16));
	SetShortRAMAddr(WRITE_PANIDL,(unsigned char)(panid%16));
	SetShortRAMAddr(WRITE_PANIDH,(unsigned char)(panid/16));
	//for(i=0;i<64;i++) SetLongRAMAddr(SECURITY_FIFO+i,0);
    SetLongRAMAddr(RFCTRL2,0x80);			//enable the RF-PLL
	SetLongRAMAddr(RFCTRL3,0x00);			//set TX for max output power
	SetLongRAMAddr(RFCTRL6,0x80);			//enable TX filter control 
	SetLongRAMAddr(RFCTRL8,0b00010000);
	SetShortRAMAddr(WRITE_BBREG2,0x38);		//CCA disabled
	SetShortRAMAddr(WRITE_BBREG6,0x40);		//Enable the packet RSSI 
	SetShortRAMAddr(WRITE_RSSITHCCA,0x00);	//Program CCA, RSSI threshold values
	SetLongRAMAddr(RFCTRL0,0x03); 			//channel 11
	//SetShortRAMAddr(WRITE_RXMCR,0x23);		//set receive filter to accept all packets and never ACK, now it is a coordinator.
    SetShortRAMAddr(WRITE_RXMCR,0x20);      //now it is strictest mode, every wrong frame would be filtered.
	SetShortRAMAddr(WRITE_RFCTL,0x04); 		//reset the RF module with new settings
	SetShortRAMAddr(WRITE_RFCTL,0x00);
	SetShortRAMAddr(WRITE_INTMSK,0xF7);		//mask all interrupts except RXIF
}

//*********************************************************************
//**** 	PHYTransmit (broadcasts packet) 		***********************
//*********************************************************************

void PHYTransmit(char *Data, unsigned char NoBytes)
{
	unsigned char i;
	SetLongRAMAddr(0,NoBytes+9);
	SetLongRAMAddr(1,NoBytes+9);	//length of data in bytes		
	SetLongRAMAddr(2,0x01);			//FCF set for short addressing, no ack request
	SetLongRAMAddr(3,0x88);
	SetLongRAMAddr(4,0x01);			//sequence number set to 1
	SetLongRAMAddr(5,0xFF);			//PAN ID set to broadcast
	SetLongRAMAddr(6,0xFF);
	SetLongRAMAddr(7,0xFF);			//destination address set to broadcast
	SetLongRAMAddr(8,0xFF);
	SetLongRAMAddr(9,0xFF);			//source address set to broadcast
	SetLongRAMAddr(10,0xFF);
	for(i=0; i<NoBytes; i++) SetLongRAMAddr(11+i,Data[i]);	//write data to Tx FIFO
	SetShortRAMAddr(WRITE_TXNMTRIG,0b00000001);		//trigger Tx FIFO
}

//*********************************************************************
//**** 	PHYReceive (Receives packet, returns number of bytes **********
//*********************************************************************

unsigned char PHYReceive(char *Data, unsigned char *Strength)
{
	unsigned char NoBytes,i;
	if(GetShortRAMAddr(READ_ISRSTS) & 0x08)		//check RXIF interrupt flag
		{
		NoBytes = GetLongRAMAddr(RX_FIFO)-11;		//read length field & subtract protocol bytes)
		for(i=0; i<NoBytes; i++) Data[i] = GetLongRAMAddr(RX_FIFO+10+i);	//get data from buffer
		*Strength = GetLongRAMAddr(RX_FIFO+NoBytes+13);	//get RSSI
		SetShortRAMAddr(WRITE_RXFLUSH,0x01);		//flush the RX fifo 	
		return(NoBytes);		//return number of data bytes - FCS
		}
	else return(0);					//no packet received*/
}

//*********************************************************************
//**** 	PHYGetRSSI (gets current RSSI)						 **********
//*********************************************************************

unsigned char PHYGetRSSI(void)
{
	unsigned char RSSICheck;
	SetShortRAMAddr(WRITE_BBREG6, 0xC0);			// set RSSI request
    RSSICheck = GetShortRAMAddr(READ_BBREG6);		// check RSSI
    while((RSSICheck & 0x01) != 0x01 || (RSSICheck & 0x80)) RSSICheck = GetShortRAMAddr(READ_BBREG6);
    RSSICheck = GetLongRAMAddr(0x210);
	return(RSSICheck);
}
