#include "TSL2561.h"

unsigned char TSL_PowerOn()
{
    unsigned char data = 0;
    I2C_Wrong = 0;
    IIC_Start();
    IIC_Send_Byte(SLA_WRITE);
    I2C_Wrong |= IIC_Wait_Ack();
    IIC_Send_Byte(TSL2561_COMMAND_BIT | TSL2561_REGISTER_CONTROL);
    I2C_Wrong |= IIC_Wait_Ack();
    IIC_Send_Byte(TSL2561_CONTROL_POWERON);
    I2C_Wrong |= IIC_Wait_Ack();
    IIC_Stop();
    /*
    IIC_Start();
    IIC_Send_Byte(SLA_WRITE);
    I2C_Wrong = IIC_Wait_Ack();
    if(I2C_Wrong == 1)
    {
        USARTOut(I2C_Wrong_Flag,strlen(I2C_Wrong_Flag));
    }
    IIC_Send_Byte(TSL2561_COMMAND_BIT | TSL2561_REGISTER_CONTROL);
    I2C_Wrong = IIC_Wait_Ack();
    if(I2C_Wrong == 1)
    {
        USARTOut(I2C_Wrong_Flag,strlen(I2C_Wrong_Flag));
    }
    __delay_us(5);
    IIC_Start();
    IIC_Send_Byte(SLA_READ);
    I2C_Wrong = IIC_Wait_Ack();
    if(I2C_Wrong == 1)
    {
        USARTOut(I2C_Wrong_Flag,strlen(I2C_Wrong_Flag));
    }
    data = IIC_Read_Byte(NACK);
    IIC_Stop();
     */
    return data;
}

void TSL_Set_Time()
{
    //TIME
    IIC_Start();
    IIC_Send_Byte(SLA_WRITE);
    I2C_Wrong |= IIC_Wait_Ack();
    IIC_Send_Byte(TSL2561_COMMAND_BIT | TSL2561_REGISTER_TIMING);
    I2C_Wrong |= IIC_Wait_Ack();
    IIC_Send_Byte(TSL2561_CLIPPING_13MS);
    I2C_Wrong |= IIC_Wait_Ack();
    IIC_Stop();
}

short unsigned int TSL_Get_Light_Channel0()
{
	short unsigned int date;
	IIC_Start();
	IIC_Send_Byte(SLA_WRITE);
    I2C_Wrong |= IIC_Wait_Ack();
	IIC_Send_Byte(TSL2561_COMMAND_BIT | TSL2561_REGISTER_CHAN0_LOW);
    I2C_Wrong |= IIC_Wait_Ack();
	__delay_us(5);
	IIC_Start();
	IIC_Send_Byte(SLA_READ);
    I2C_Wrong |= IIC_Wait_Ack();
	date = IIC_Read_Byte(ACK);
	date+= (IIC_Read_Byte(NACK)<<8);
	IIC_Stop();
	return date;
}