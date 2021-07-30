/* 
 * File:   AMG8833.h
 * Author: asus
 *
 * Created on 2021?6?19?, ??9:29
 */

#ifndef AMG8833_H
#define	AMG8833_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef __AMG8833_H
#define __AMG8833_H

#include "xc.h"
 
#define AMG88xx_PIXEL_TEMP_CONVERSION 0.25
#define AMG88xx_THERMISTOR_CONVERSION 0.0625
    
#define AMG88xx_ADR    0xD0 //5?????????
//#define DEBUG

char Test[13];
float Temp_Value;
float Temp_Pixel[64];
short unsigned int debugvalue;
 
enum
{
	AMG88xx_PCTL = 0x00,
	AMG88xx_RST = 0x01,
	AMG88xx_FPSC = 0x02,
	AMG88xx_INTC = 0x03,
	AMG88xx_STAT = 0x04,
	AMG88xx_SCLR = 0x05,
	//0x06 reserved
	AMG88xx_AVE = 0x07,
	AMG88xx_INTHL = 0x08,
	AMG88xx_INTHH = 0x09,
	AMG88xx_INTLL = 0x0A,
	AMG88xx_INTLH = 0x0B,
	AMG88xx_IHYSL = 0x0C,
	AMG88xx_IHYSH = 0x0D,
	AMG88xx_TTHL = 0x0E,
	AMG88xx_TTHH = 0x0F,
	AMG88xx_INT_OFFSET = 0x010,
	AMG88xx_PIXEL_OFFSET = 0x80
};
 
enum power_modes{
	AMG88xx_NORMAL_MODE = 0x00,
	AMG88xx_SLEEP_MODE = 0x01,
	AMG88xx_STAND_BY_60 = 0x20,
	AMG88xx_STAND_BY_10 = 0x21
};
 
enum sw_resets {
	AMG88xx_FLAG_RESET = 0x30,
	AMG88xx_INITIAL_RESET = 0x3F
};
 
enum frame_rates {
	AMG88xx_FPS_10 = 0x00,
	AMG88xx_FPS_1 = 0x01
};
 
enum int_enables{
	AMG88xx_INT_DISABLED = 0x00,
	AMG88xx_INT_ENABLED = 0x01
};
 
enum int_modes {
	AMG88xx_DIFFERENCE = 0x00,
	AMG88xx_ABSOLUTE_VALUE = 0x01
};

unsigned char AMG_IIC_Write_1Byte(unsigned char SlaveAddress, unsigned char REG_Address,unsigned char REG_data);
unsigned char AMG_IIC_Read_1Byte(unsigned char SlaveAddress, unsigned char REG_Address,unsigned char *REG_data);
unsigned char AMG_I2C_Write_nByte(unsigned char SlaveAddress, unsigned char REG_Address, unsigned char *buf, short unsigned int len);
unsigned char AMG_I2C_Read_nByte(unsigned char SlaveAddress, unsigned char REG_Address, unsigned char *buf, short unsigned int len);
void AMG8833_Init(void);
float signedMag12ToFloat(short unsigned int val);
float AMG88xx_ReadThermistor(void);
void AMG88xx_ReadPixels(float *buf, unsigned char size);

void AMG88xx_GetTemp(void *Data);
void AMG88xx_GetPixel(void *Data);
 
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* AMG8833_H */

