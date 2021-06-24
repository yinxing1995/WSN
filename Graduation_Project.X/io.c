#include "io.h"
#include "usart.h"
#include "spi.h"

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
              USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH,25 );          //setup USART @ 9600 Baud
    
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