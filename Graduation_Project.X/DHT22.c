#include "DHT22.h"
#include <xc.h>
#include "io.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned char repeated = 0;

void DHT22_Init()
{
    if(repeated)
    {
        repeated != repeated;
        return;
    }
    
    repeated != repeated;
    int i = 0;
    while(i<10)
    {
        __delay_ms(150);
        i++;
    }
}

void DHT22_Start(void)
{
	DH_OUT(); 
    DH_DATA = 1; 
    __delay_ms(50);
	DH_DATA = 0; 
    __delay_us(1);
	__delay_ms(18);
	DH_DATA = 1; 
	__delay_us(30);
}

unsigned char DHT22_Ack(void)
{
	unsigned char cnt = 0;
	DH_IN();//??IO?????
    __delay_us(1);
    DH_DATA = 1;
    __delay_us(1);
	if (!DH_DATA) //????????
	{
        while(!DH_DATA);
        while(DH_DATA);
        return SUCCESS;   
	}
    
	else
	{
#ifdef DEBUG
        Uprintf("No response\r\n");
#endif
		return FAILED;
	}
}

static void bit_clear(unsigned char *data, unsigned char i)
{
    switch(i)
    {
        case 0:
            *data &= 0b11111110;
            break;
        case 1:
            *data &= 0b11111101;
            break;
        case 2:
            *data &= 0b11111011;
            break;
        case 3:
            *data &= 0b11110111;
            break;
        case 4:
            *data &= 0b11101111;
            break;
        case 5:
            *data &= 0b11011111;
            break;
        case 6:
            *data &= 0b10111111;
            break;           
        case 7:
            *data &= 0b01111111;
            break;
        default:
            break;
    }
}

static void bit_set(unsigned char *data, unsigned char i)
{
    switch(i)
    {
        case 0:
            *data |= 0b00000001;
            break;
        case 1:
            *data |= 0b00000010;
            break;
        case 2:
            *data |= 0b00000100;
            break;
        case 3:
            *data |= 0b00001000;
            break;
        case 4:
            *data |= 0b00010000;
            break;
        case 5:
            *data |= 0b00100000;
            break;
        case 6:
            *data |= 0b01000000;
            break;           
        case 7:
            *data |= 0b10000000;
            break;
        default:
            break;
    }
}

static void DHT22_Read_Byte(unsigned char *bits)
{
    int i = 0,timediff;
    for(i=0;i<40;i++)
    {
        timediff = 0;
        while(!DH_DATA);
        while(DH_DATA)
        {
            timediff++;
        }
        bits[i] = timediff;
    }
    __delay_us(100);
}

unsigned char Read_DHT22_Data(DHT22_Data * TandH)
{
	unsigned char bits[40];   
    unsigned char value[5];
    unsigned char *p1 = bits,*p2 = value;
    int i = 0,j = 0;
    
	DHT22_Start();
	if(DHT22_Ack() == 0)
	{
        DHT22_Read_Byte(bits);
	}
    else
        return 1;
    for(i=0;i<5;i++)
    {
        for(j=0;j<8;j++)
        {
            if(*p1<10)
                bit_clear(p2,7-j);
            else
                bit_set(p2,7-j);
            p1++;
        }
        p2++;
    }
    p1 = NULL;
    p2 = NULL;
    TandH->RH_int = value[0];
    TandH->RH_deci = value[1];
    TandH->Temp_int = value[2];
    TandH->Temp_deci = value[3];
    TandH->check_sum = value[4];
    TandH->sum = value[0]+value[1]+value[2]+value[3];
    if(TandH->check_sum == TandH->sum)
	{
        TandH->RH = (float)((256 * TandH->RH_int + TandH->RH_deci) / 10.0f);
		TandH->Temp = (float)((256 * TandH->Temp_int + TandH->Temp_deci) / 10.0f);
		return SUCCESS;
	}
	else
		return FAILED;
}

void Read_DHT22_Temp(void *Data)
{
    if(!repeated)
        Read_DHT22_Data(&DHT_DATA);
    //memcpy(Data,&(DHT_DATA.Temp),sizeof(float));
    repeated != repeated;
}

void Read_DHT22_RH(void *Data)
{
    if(!repeated)
        Read_DHT22_Data(&DHT_DATA);
    //memcpy(Data,&(DHT_DATA.RH),sizeof(float));
    repeated != repeated;
}
