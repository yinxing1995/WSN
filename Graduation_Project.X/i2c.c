#include "i2c.h"
#include <xc.h>

void Init_I2C()
{
    TRISBbits.RB7 = 0;//output
    SDA_OUT();
    IIC_SDA = 1;
    IIC_SCL = 1;
}

//IIC start bit
void IIC_Start(void)
{
    SDA_OUT();     //sda gets ready
    IIC_SDA=1;
    __delay_us(1); //inserted
    IIC_SCL=1;
    __delay_us(4);
    IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
    __delay_us(4);
    IIC_SCL=0;//signal ends
    __delay_us(1); //inserted
}   

//IIC stop bit
void IIC_Stop(void)
{
    SDA_OUT();      //sda gets ready
    IIC_SCL=0;
    __delay_us(1);  //inserted
    IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
    __delay_us(4);
    IIC_SCL=1;
    __delay_us(1);  //inserted
    IIC_SDA=1;//signal ends
    __delay_us(4);    
}

//IIC wait for ACK signal
unsigned char IIC_Wait_Ack(void)
{
    unsigned char ucErrTime=0;
    SDA_IN();      //SDA read
    IIC_SDA=1;__delay_us(1);    
    IIC_SCL=1;__delay_us(1);  
    while(IIC_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL=0;//end
    __delay_us(1);  //inserted
    return 0;  
}

//ACK
void IIC_Ack(void)
{
    IIC_SCL=0;
    __delay_us(1);//inserted
    SDA_OUT();
    __delay_us(1);//inserted
    IIC_SDA=0;
    __delay_us(2);
    IIC_SCL=1;
    __delay_us(2);
    IIC_SCL=0;
    __delay_us(1);  //inserted
}

//NACK
void IIC_NAck(void)
{
    IIC_SCL=0;
    __delay_us(1);//inserted
    SDA_OUT();
    __delay_us(1);//inserted
    IIC_SDA=1;
    __delay_us(2);
    IIC_SCL=1;
    __delay_us(2);
    IIC_SCL=0;
    __delay_us(1);  //inserted
}    

//IIC Send
void IIC_Send_Byte(unsigned char txd)
{                        
    unsigned char t;   
    SDA_OUT();      
    IIC_SCL=0;//ready for rising edge
    __delay_us(1);  //inserted
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
        if((txd&0x80)>>7)
            IIC_SDA=1;
        else
            IIC_SDA=0;
        txd<<=1;    
        __delay_us(2); 
        IIC_SCL=1;
        __delay_us(2); 
        IIC_SCL=0; 
        __delay_us(2);
    }  
}

//Read with ACK processing 
unsigned char IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();//SDA?????
    __delay_us(2);
    for(i=0;i<8;i++ )
    {
        IIC_SCL=0; 
        __delay_us(2);
        IIC_SCL=1;
        __delay_us(2); 
        receive<<=1;
        if(IIC_SDA)receive++;   
        __delay_us(2); 
    }  
    if (!ack)
        IIC_NAck();//??nACK
    else
        IIC_Ack(); //??ACK   
    return receive;
}
