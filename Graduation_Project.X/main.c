#include <xc.h>
 

 
#include <stdlib.h>                              //standard library
#include <stdio.h>
#include <spi.h>                                  //serial peripheral interface functions
#include <delays.h>         //time delay functions
#include <usart.h>                              //USART functions
#include <string.h>                             //string functions
#include <adc.h>
#include <timers.h>
#include "MRF24J40.h"                       //driver function definitions for MRF24J40 RF transceiver
#include <p18c452.h>
#include "AMG8833.h"
#include "i2c.h"
#include "TSL2561.h"
#include "crc.h"
#include "io.h"
#include "protocol.h"
 

#define AMG8833
//#define TSL2561

char Text[20];



 
         

volatile char rev = 0;
volatile char timerflag = 0;

void interrupt myISR()
{
    if(PIR1bits.RCIF)
    {
        rev = ReadUSART();
        PIR1bits.RCIF = 0;
        USARTOut(&rev,1);
    }
    if(INTCONbits.TMR0IF)
    {
        timerflag = 1;
        INTCONbits.TMR0IF = 0;
    }
}


void main(void)
{
    Init_IO();
    __delay_ms(150);
    __delay_ms(150);
    __delay_ms(150);
    OpenTimer0( TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_16);                   //setup timer 0 with prescaler x16
    WriteTimer0(3036);
    
    Init_Attributes();
#ifdef TSL2561
    AddEndpoint(LightStrength,_INT,1,&Light_Strngth,READONLY,TSL_Init,TSL_GetData,0);
#elif defined AMG8833
    AddEndpoint(Temperature,_FLOAT,1,&Temp_Value,READONLY,AMG8833_Init,AMG88xx_GetTemp,0);
    AddEndpoint(Temperature,_FLOAT,64,&Temp_Pixel,READONLY,NULL,AMG88xx_GetPixel,0);
    //AddEndpoint(0,0,0,0,0,0,0,0);
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
        if(timerflag)
        {
            timerflag = 0;
            EndpointGetData();
            EndpointReport();
        }
    } 
}


