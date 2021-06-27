/* 
 * File:   DHT22.h
 * Author: asus
 *
 * Created on 2021?6?26?, ??1:38
 */

#ifndef DHT22_H
#define	DHT22_H


#ifdef	__cplusplus
extern "C" {
#endif

#define _XTAL_FREQ 4000000
    
//#define DEBUG
//DH_DATA RD1
#define DH_IN() TRISDbits.RD1 = 1;\
                //INTCON2bits.nRBPU = 1;//enable pull-ups
    
#define DH_OUT() TRISDbits.RD1 = 0;
    
#define DH_DATA PORTDbits.RD1

enum ReturnValue
{
    SUCCESS = 0x00,
    FAILED = 0x01,
};
    
typedef struct DHT22_Data_TypeDef
{
    unsigned char RH_int;
    unsigned char RH_deci;
    unsigned char Temp_int;
    unsigned char Temp_deci;
    unsigned char check_sum;
    unsigned char sum;
    float RH;
    float Temp;
}DHT22_Data;

DHT22_Data DHT_DATA;

void DHT22_Init(void);
void DHT22_Start(void);
unsigned char DHT22_Ack(void);
unsigned char Read_DHT22_Data(DHT22_Data *);
void Read_DHT22_Temp(void *Data);
void Read_DHT22_RH(void *Data);


#ifdef	__cplusplus
}
#endif

#endif	/* DHT22_H */

