#include "io.h"
#include "usart.h"
#include "spi.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void Init_IO(void)
{
    PORTA = 0x04;                                                          //PORTA initially all zeros except RA2 (TC77 chip select)
    TRISA = 0xF8;                                                          //RA0 and RA1 outputs (LEDs), RA2 Output (TC77 CS), rest inputs
    TRISB = 0xFF;                                                          //PORTB all inputs (RB0 is interrupt, RB4 and RB5 are push buttons)
    INTCON2bits.RBPU = 0;                                                  //enable pull up resistors on PORTB
    ADCON0 = 0x1C;                                                         //turn off analog input
 
    PORTCbits.RC0 = 1;                                                     //Chip select (/CS) initially set high (MRF24J40)
    TRISCbits.TRISC0 = 0;                                                  //Output: /CS
    PORTCbits.RC1 = 1;                                                                 //WAKE initially set high (MRF24J40)
    TRISCbits.TRISC0 = 0;                                                  //Output: WAKE
    PORTCbits.RC2 = 1;                                                                 //RESETn initially set high (MRF24J40)
    TRISCbits.TRISC2 = 0;                                                  //output: RESETn    
    
    INTCONbits.INT0IF = 0;                                                 //clear the interrupt flag (INT0 = RB0)
    INTCONbits.INT0IE = 1;                                                 //enable INT0
    RCONbits.IPEN = 1;                                                     //enable interrupt priorities
    INTCONbits.GIEH = 1;                                                   //global interrupt enable
    OSCCONbits.IDLEN = 1;                                                  //enable idle mode (when Sleep() is executed)
    
    //INTCONbits.PEIE = 1; // Enable Peripheral interrupt
    //PIR1bits.RCIF = 0; // Clear
 
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE &
              USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH,12 );          //setup USART @ 19200 Baud
    
    OpenSPI(SPI_FOSC_4,MODE_00,SMPMID);               //setup SPI bus (SPI mode 00, 1MHz SCLK) (MRF24J40)
}

void USARTOut(const char *data, char bytes)
{
    int i;
    for(i=0; i<bytes; i++)
    {
        while(BusyUSART());
        WriteUSART(data[i]);
    }
}

static void printint(int data)
{
    char p[MAX_SIZE] = {0};
    sprintf(p,"%d",data);
    USARTOut(p,strlen(p));
}

static printfloat(float data)
{
    char p[MAX_SIZE] = {0};
    sprintf(p,"%.2f",data);
    USARTOut(p,strlen(p));
}

static printchar(char data)
{
    USARTOut(&data,1);
}

void Uprintf(const char *fmt, ...)
{
    va_list ap; /* points to each unnamed arg in turn */ 
	char *p, *sval;
	int ival;
	float dval;
	va_start(ap, fmt); /* make ap point to 1st unnamed arg */ 
	for (p = fmt; *p; p++) {
		if (*p != '%') { 
			printchar(*p);
			continue; 
		}
		switch (*++p) { 
			case 'd':
				ival = va_arg(ap, int); printint(ival); 
				break;
			case 'f':
				dval = va_arg(ap, double); printfloat(dval);
				break;
			case 's':
				for (sval = va_arg(ap, char *); *sval; sval++)
					printchar(*sval); 
				break;
			default: 
				printchar(*p);
				break; 
		}
	}
	va_end(ap); /* clean up when done */ 
}