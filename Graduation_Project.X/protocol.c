#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "protocol.h"
#include "io.h"
#include "wsn.h"
#include <string.h>
#include "ringbuffer.h"

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
        //if(Array[i].DataType == _INT32)
        Array[i].GetData(Array[i].Data);
        //else
            //Array[i].GetData(Array[i].Data);
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
        case _INT32:
            datasize = sizeof(signed long);
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
    if(NODE_ID == 1)
    {
        USARTOut("Frame",strlen("Frame"));
        USARTOut(Message, currentpos);
    }
    else
    {
        WSNTransmit(Message, currentpos, PAN_ID, destination_addr, source_addr);
    }
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


void MessageReport(unsigned char * buf)
{
    /*
    if(!buf)
    {
        USARTOut("No info\r\n",strlen("No info\r\n"));
        return;
    }
    else
    {
        USARTOut("Get\r\n",strlen("Get\r\n"));
    }
    */
    if(!buf) return;
    unsigned char temp = 0;
    unsigned short int i,buflen;
    memcpy(&buflen,buf,sizeof(buflen));
    for(i=0;i<buflen-1;i++)
    {
        temp += buf[i];
    }
    if(temp != buf[i])
        return;
    else
    {
        USARTOut("Frame",strlen("Frame"));
        USARTOut(buf, buflen);
    }   
}

enum states
{
    Init = 0x00,
    GetLen = 0x01,
    FramePick = 0x02,
    DataUpdate = 0x03,
};

unsigned char Mask = 0;
unsigned char LenMask = 0;

unsigned short int FrameLen;
unsigned char frameflag;

int State = Init;
char fbuf[50];

void CommandPro(void)
{
    unsigned short int i = 0;
    unsigned char checksum = 0;
    switch(State)
    {
        case Init:
            if(Mask == 0)
            {
                if(BufferRead(&frameflag,1))
                    break;
                if(frameflag == 70)
                    Mask = 1;
            }
            else if(Mask == 1)
            {
                if(BufferRead(&frameflag,1))
                    break;
                if(frameflag == 70)
                    Mask = 1;
                else if(frameflag == 114)
                    Mask = 2;
                else
                    Mask = 0;
            }
            else if(Mask == 2)
            {
                if(BufferRead(&frameflag,1))
                    break;
                if(frameflag == 70)
                    Mask = 1;
                else if(frameflag == 97)
                    Mask = 3;
                else
                    Mask = 0;
            }
            else if(Mask == 3)
            {
                if(BufferRead(&frameflag,1))
                    break;
                if(frameflag == 70)
                    Mask = 1;
                else if(frameflag == 109)
                    Mask = 4;
                else
                    Mask = 0;
            }
            else if(Mask == 4)
            {
                 if(BufferRead(&frameflag,1))
                    break;
                if(frameflag == 70)
                    Mask = 1;
                else if(frameflag == 101)
                    Mask = 5;
                else
                    Mask = 0;
            }
            else if(Mask == 5)
            {
                Mask = 0;
                State = GetLen;
            }
            break;
        case GetLen:
            if(BufferRead(&frameflag,1))
                break;
            if(LenMask == 0)
            {
                FrameLen += frameflag;
                LenMask = 1;
            }
            else
            {
                FrameLen += frameflag << 8;
                State = FramePick;
                //Uprintf("\r\nLen = %d\r\n",FrameLen);
                LenMask = 0;
            }
            break;
        case FramePick:
            if(BufferSeek(&fbuf[0],FrameLen-sizeof(FrameLen)))
                break;
            for(i=0;i<FrameLen-sizeof(FrameLen)-1;i++)
            {
                BufferRead(&fbuf[i],1);
                //Uprintf(" %d ",fbuf[i]);
                checksum += fbuf[i];
            }
            BufferRead(&fbuf[i],1);
            checksum += ((unsigned char)FrameLen + (unsigned char)(FrameLen>>8));
            if(checksum == fbuf[i])
            {
                //Uprintf("\r\nFramePicked\r\n");
                State = DataUpdate;
            }
            else
            {
                //Uprintf("\r\nChecksum incorrect\r\n");
                FrameLen = 0;
                memset(fbuf,0,FrameLen);
                State = Init;
            }
            break;
            
        case DataUpdate:
            memmove(&fbuf[sizeof(FrameLen)],&fbuf[0],FrameLen - sizeof(FrameLen));
            memcpy(&fbuf[0],&FrameLen,sizeof(FrameLen));     
            //USARTOut("Frame", strlen("Frame"));
            //USARTOut(fbuf, FrameLen);
            if(NODE_ID == 1)
            {
                unsigned short int addr = 0;
                if(fbuf[3] == 2)addr = SHORT_ADDR_2;
                else if(fbuf[3] == 3)addr = SHORT_ADDR_3;
                else if(fbuf[3] == 4)addr = SHORT_ADDR_4;
                else goto exit;
                WSNTransmit(fbuf, FrameLen, PAN_ID, addr, SHORT_ADDR_1);
                //Uprintf("Sent!\r\n");
            }
    exit:   FrameLen = 0;
            State = Init;
            break;
        default:
            break;
    }
}