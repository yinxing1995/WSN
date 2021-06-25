#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "protocol.h"
#include "io.h"
#include <string.h>

ClusterArray Array[MAX_Num];
int NumofElement;

void Init_Attributes(void)
{
    NumofElement = 0;
}

void AddEndpoint(unsigned char ClusterID, unsigned char DataType, unsigned char DataLength,void *Data,unsigned char ControlFlag, void (*InitFun)(void),
                    void (*GetData)(unsigned char *Data), 
                    void (*ControlFun)(unsigned char *Data))
{
    Array[NumofElement].ClusterID = ClusterID;
    Array[NumofElement].DataType = DataType;
    Array[NumofElement].DataLength = DataLength;
    Array[NumofElement].ControlFlag = ControlFlag;
    Array[NumofElement].InitFun = InitFun;
    Array[NumofElement].GetData = GetData;
    Array[NumofElement].Data = Data;
    Array[NumofElement].ControlFun = ControlFun;
    Array[NumofElement].Endpoint = NumofElement;
    NumofElement++;
}

void InitEndpoint()
{
    int i = 0;
    for(i=0;i<NumofElement;i++)
    {
        if(Array[i].InitFun)
            Array[i].InitFun();
    }
}

void EndpointGetData()
{
    int i = 0;
    for(i=0;i<NumofElement;i++)
    {
        if(Array[i].DataType == _INT)
            Array[i].GetData(Array[i].Data);
        else
            Array[i].GetData(Array[i].Data);
    }
}


void EndpointReport()
{
    int i,q = 0;
#ifdef DEBUG
    Uprintf("Num = %d \r\n",NumofElement);
    for(i=0;i<NumofElement;i++)
    {
        if(Array[i].DataType == _INT)
        {
            int *p = (int *)(Array[i].Data);
            Uprintf("Value = ");
            for(q=0;q<Array[i].DataLength;q++)
            {
                Uprintf(" %d ",*p++);
            }            
            Uprintf("\r\n");
        }
        else
        {
            float *p = (float *)(Array[i].Data);
            Uprintf("Float value = ");
            for(q=0;q<Array[i].DataLength;q++)
            {
                Uprintf(" %f ",*p++);
            }            
            Uprintf("\r\n");
        }
    }
#else
    
#endif  
}