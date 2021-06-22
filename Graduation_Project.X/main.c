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
 

//#define AMG8833
//#define TSL2561

char Text[20];



 
         

char b = 0;

void interrupt myISR()
{
    b = ReadUSART();
    PIR1bits.RCIF = 0;
    USARTOut(&b,1);
}


void main(void)
{
    Init_IO();
    __delay_ms(150);
    __delay_ms(150);
    __delay_ms(150);
    //OpenTimer0( TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_16);                   //setup timer 0 with prescaler x16
    //WriteTimer0(3036);
#ifdef TSL2561
    Init_I2C();
    TSL_PowerOn();
    TSL_Set_Time();
    while(1)
    {
        if(INTCONbits.TMR0IF)                                                                                
        {
            INTCONbits.TMR0IF = 0;
            short unsigned int data = TSL_Get_Light_Channel0();
            sprintf(Text,"%d \r\n",data);
            USARTOut(Text,strlen(Text));
        }
    }
#elif defined AMG8833
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
#else
    while(1)
    {
        ;
    }
#endif    
}


