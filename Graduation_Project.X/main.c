#include <xc.h>
 

 
#include <stdlib.h>                              //standard library
#include <stdio.h>
#include <spi.h>                                  //serial peripheral interface functions
#include <usart.h>                              //USART functions
#include <string.h>                             //string functions
#include <timers.h>
#include "MRF24J40.h"                       //driver function definitions for MRF24J40 RF transceiver
#include <p18c452.h>
#include "AMG8833.h"
//#include "i2c.h"
#include "TSL2561.h"
//#include "crc.h"
#include "io.h"
#include "protocol.h"
#include "DHT22.h"
#include "wsn.h"

//#define DHT22
//#define AMG8833
#define TSL2561


volatile char rev = 0;

void interrupt myISR()
{
    if(PIR1bits.RCIF)
    {
        rev = ReadUSART();
        PIR1bits.RCIF = 0;
        USARTOut((const char *)&rev,1);
    }
}


void main(void)
{
    Init_IO();
    Node_Init();
    OpenTimer0(TIMER_INT_OFF & T0_16BIT & T0_SOURCE_INT & T0_PS_1_16);                   //setup timer 0 with prescaler x16
    WriteTimer0(3036);
    
    Init_Attributes();
#ifdef TSL2561
    AddEndpoint(LightStrength,_INT,1,&Light_Strngth,READONLY,TSL_Init,TSL_GetData,0);
#elif defined AMG8833
    AddEndpoint(Temperature,_FLOAT,1,(void *)&Temp_Value,READONLY,AMG8833_Init,AMG88xx_GetTemp,0);
    AddEndpoint(Temperature,_FLOAT,64,(void *)Temp_Pixel,READONLY,NULL,AMG88xx_GetPixel,0);
#elif defined DHT22
    AddEndpoint(Temperature,_FLOAT,1,(void *)&(DHT_DATA.Temp),READONLY,DHT22_Init,Read_DHT22_Temp,0);
    AddEndpoint(Humidity,_FLOAT,1,(void *)&(DHT_DATA.RH),READONLY,DHT22_Init,Read_DHT22_RH,0);
#endif
    InitEndpoint();
/*
// TSL2561
    Init_I2C();
    TSL_PowerOn();
    TSL_Set_Time();
    while(1)
    {
        if(timerflag)                                                                                
        {
            timerflag = 0;
            short unsigned int data = TSL_Get_Light_Channel0();
            sprintf(Text,"%d \r\n",data);
            USARTOut(Text,strlen(Text));
        }
    }
// defined AMG8833
    AMG8833_Init();
    while(1)
    {
        if(INTCONbits.TMR0IF)                                                                                
        {
            int i = 0;
            float data = 0;
            I2C_Wrong = 0;
            data = AMG88xx_ReadThermistor();
            INTCONbits.TMR0IF = 0;                                                                     
            PORTA = 0x05;
            if(data == 0)
                sprintf(Text,"I2C...\r\n");
            else
                sprintf(Text,"Temp = %f C\r\n",data);
            USARTOut(Text,strlen(Text));
            //debug
            debugvalue <<= 4;
            for(i=0;i<12;i++)
            {
                if(debugvalue & 0x8000)
                    Test[i] = '1';
                else
                    Test[i] = '0';
                debugvalue <<= 1;
            }
            sprintf(Text,"Binary = %s  I2C Wrong = %d\r\n",Test,I2C_Wrong);
            USARTOut(Text,strlen(Text));
            PORTA = 0x04;
        }           
    }
*/
    while(1)
    {
        if(INTCONbits.TMR0IF)
        {
            Report_time++;
            INTCONbits.TMR0IF = 0;
        }
        if(Report_time == REPORT_TIMER)
        {
            Report_time = 0;
            EndpointGetData();
            EndpointReport();
        }
        if(NODE_ID == 1)
        {
                MessageReport(MessageGet());
        }
    }
}


