#include "AMG8833.h"
#include "i2c.h"

//AMG8833 functions
unsigned char AMG_IIC_Write_1Byte(unsigned char SlaveAddress, unsigned char REG_Address,unsigned char REG_data)
{
	IIC_Start();
	IIC_Send_Byte(SlaveAddress);
	I2C_Wrong |= IIC_Wait_Ack();
	IIC_Send_Byte(REG_Address);
	I2C_Wrong |= IIC_Wait_Ack();	
	__delay_us(5);
	IIC_Send_Byte(REG_data);
	I2C_Wrong |= IIC_Wait_Ack();	
	IIC_Stop();
 
	return 0;
}


unsigned char AMG_IIC_Read_1Byte(unsigned char SlaveAddress, unsigned char REG_Address,unsigned char *REG_data) 
{
	IIC_Start();
	IIC_Send_Byte(SlaveAddress);//????
	I2C_Wrong |= IIC_Wait_Ack();
	IIC_Send_Byte(REG_Address);
	I2C_Wrong |= IIC_Wait_Ack();
	__delay_us(5);
	IIC_Start(); 
	IIC_Send_Byte(SlaveAddress|0x01);//????
	I2C_Wrong |= IIC_Wait_Ack();
	*REG_data = IIC_Read_Byte(NACK);
	IIC_Stop();
    
	return 0;
}


unsigned char AMG_I2C_Write_nByte(unsigned char SlaveAddress, unsigned char REG_Address, unsigned char *buf, short unsigned int len)
{
	IIC_Start();
	IIC_Send_Byte(SlaveAddress);//????
	I2C_Wrong |= IIC_Wait_Ack();
	IIC_Send_Byte(REG_Address);
	I2C_Wrong |= IIC_Wait_Ack();
	while(len--)
	{
		IIC_Send_Byte(*buf++);
		I2C_Wrong |= IIC_Wait_Ack();
	}
	IIC_Stop();
 
	return 0;
}

//I2C???????
unsigned char AMG_I2C_Read_nByte(unsigned char SlaveAddress, unsigned char REG_Address, unsigned char *buf, short unsigned int len)
{
	IIC_Start();
	IIC_Send_Byte(SlaveAddress);//????
	I2C_Wrong |= IIC_Wait_Ack();
	IIC_Send_Byte(REG_Address);
	I2C_Wrong |= IIC_Wait_Ack();
	__delay_us(5);
	IIC_Start(); 
	IIC_Send_Byte(SlaveAddress|0x01);//????
	I2C_Wrong |= IIC_Wait_Ack();
	while(len)
	{
		
		if(1 == len)
		{
			*buf = IIC_Read_Byte(NACK);
		}
		else
		{
			*buf = IIC_Read_Byte(ACK);
		}
		buf++;
		len--;
	}
	IIC_Stop();

	return 0;
}



void AMG8833_Init(void)
{
	Init_I2C();           //IIC???
	
	//enter normal mode
	AMG_IIC_Write_1Byte(AMG88xx_ADR,AMG88xx_PCTL,AMG88xx_NORMAL_MODE);
	
	//software reset
	AMG_IIC_Write_1Byte(AMG88xx_ADR,AMG88xx_RST,AMG88xx_INITIAL_RESET);
	
	//set to 10 FPS
	AMG_IIC_Write_1Byte(AMG88xx_ADR,AMG88xx_FPSC,AMG88xx_FPS_10);
}


float signedMag12ToFloat(short unsigned int val)
{
	//take first 11 bits as absolute val
	short unsigned int absVal = (val & 0x7FF);
	
	return (val & 0x800) ? (0 - (float)absVal) : (float)absVal ;
}


float AMG88xx_ReadThermistor(void)
{
	unsigned char raw[2];
	short unsigned int recast;
	
	AMG_I2C_Read_nByte(AMG88xx_ADR,AMG88xx_TTHL, raw, 2);
	recast = ((short unsigned int)raw[1] << 8) | ((short unsigned int)raw[0]);
    debugvalue = recast;//debug
	return signedMag12ToFloat(recast) * AMG88xx_THERMISTOR_CONVERSION;
}


void amg88xx_readPixels(float *buf, unsigned char size)
{
	short unsigned int recast;
	float converted;
	unsigned char rawArray[128],i;
	
	AMG_I2C_Read_nByte(AMG88xx_ADR,AMG88xx_PIXEL_OFFSET,rawArray,128);
	
	for(i=0; i<size; i++)
	{
		unsigned char pos = i << 1;
		recast = ((short unsigned int)rawArray[pos + 1] << 8) | ((short unsigned int)rawArray[pos]);        
		
		converted = signedMag12ToFloat(recast) * AMG88xx_PIXEL_TEMP_CONVERSION;
		buf[i] = converted;
	}
}
