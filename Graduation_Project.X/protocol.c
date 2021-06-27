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
                    void (*GetData)(unsigned char *Data), void (*ControlFun)(unsigned char *Data))
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

static void GenerateMessage(ClusterArray *pointer, unsigned char type)
{
    unsigned char datasize = 0,checksum = 0;
    unsigned short int length = 0;
    unsigned short int currentpos = 0;
    int i = 0;
    switch(pointer->DataType)
    {
        case _INT:
            datasize = sizeof(int);
            break;
        case _FLOAT:
            datasize = sizeof(float);
            break;
        default:
            return;
    }
    if(type == REPORT | CHECK)
    {
        (type == REPORT)?(length = 10 + (pointer->DataLength)*datasize):(length = 10);
        memcpy(Message,&length,sizeof(length));
        currentpos += sizeof(length);
        Message[currentpos] = type;
        currentpos += 1;
        Message[currentpos] = (unsigned char)NODE_ID;
        currentpos += 1;
        Message[currentpos] = pointer->ClusterID;
        currentpos += 1;
        Message[currentpos] = pointer->Endpoint;
        currentpos += 1;
        Message[currentpos] = pointer->ControlFlag;
        currentpos += 1;
        Message[currentpos] = pointer->DataType;
        currentpos += 1;
        Message[currentpos] = pointer->DataLength;
        currentpos += 1;
        if(type == REPORT)
        {
            char *buf = (char *)(pointer->Data);
            //memcpy(Message[currentpos],buf,datasize*(pointer->DataLength));
            //currentpos += datasize*(pointer->DataLength);
            for(i=0;i<datasize*(pointer->DataLength);i++)
            {
                Message[currentpos] = *(buf++);
                currentpos++;
            }
        }
         
    }
    else if(type == NETSTATUS)
    {
        //wait to be done.
    }
    else
        return;
    for(i=0;i<currentpos;i++)
    {
        checksum += Message[i];
    }
    Message[currentpos] = checksum;
    currentpos += 1;
    USARTOut(Message, currentpos);
    //Uprintf("Value = %f",*(float *)(pointer->Data));
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
    for(i=0;i<NumofElement;i++)
    {
        GenerateMessage(&Array[i], REPORT);
        for(q=0;q<50;q++)
        {
            NOP();
        }
    }
#endif  
}