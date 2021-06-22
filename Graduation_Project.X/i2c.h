/* 
 * File:   i2c.h
 * Author: asus
 *
 * Created on 2021?6?19?, ??9:34
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

//SDA RB6
//SCL RB7

#define _XTAL_FREQ 4000000
#define I2C_Wrong_Flag "I2C Wrong"
    
    
#define SDA_IN() TRISBbits.RB6 = 1;\
                     INTCON2bits.nRBPU = 1;//enable pull-ups

#define SDA_OUT() TRISBbits.RB6 = 0;\
                      
#define IIC_SDA PORTBbits.RB6
#define IIC_SCL PORTBbits.RB7

#define NOP() asm("nop")

#define ACK 1
#define NACK 0
    
unsigned char I2C_Wrong;

void Init_I2C();
void IIC_Start(void);
void IIC_Stop(void);
unsigned char IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(unsigned char txd);
unsigned char IIC_Read_Byte(unsigned char ack);


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

