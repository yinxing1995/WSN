/* 
 * File:   protocol.h
 * Author: asus
 *
 * Created on 2021?6?22?, ??10:42
 */

#ifndef PROTOCOL_H
#define	PROTOCOL_H

#ifdef	__cplusplus
extern "C" {
#endif

//UploadFrame:Length|FrameType|NodeID|Cluster|Endpoint|Controllable|Datatype|DataLength|Data|CheckSum
//CheckFrame:Length|FrameType|NodeID|Cluster|Endpoint|Controllable|Datatype|DataLength|CheckSum
//Construction:Length|FrameType|"Initialized"|CheckSum   

#define MAX_Num 5
#define DEBUG

enum
{
    Temperature = 0x00,
    TemperatureArray = 0x01,
    LightStrength = 0x02,
    InfraredStrength = 0x03,
    Humidity = 0x04,
    OnOff = 0x05,
};

enum
{
    READWRITE = 0x00,
    READONLY =0x01,
};

enum
{
    _INT = 0x00,
    _FLOAT = 0x01,
};


typedef struct Cluster
{
    unsigned char ClusterID;
    //char * Description;
    unsigned char Endpoint;
    unsigned char DataType;
    unsigned char DataLength;
    void * Data;
    unsigned char ControlFlag;
    void (*InitFun)(void);
    void (*GetData)(unsigned char *Data);
    void (*ControlFun)(unsigned char *Data);
}ClusterArray;

void Init_Attributes(void);
void AddEndpoint(unsigned char ClusterID, unsigned char DataType, unsigned char DataLength,void *Data,unsigned char ControlFlag, void (*InitFun)(void),
                    void (*GetData)(unsigned char *Data), 
                    void (*ControlFun)(unsigned char *Data));
void InitEndpoint();
void EndpointGetData();
void EndpointReport();


#ifdef	__cplusplus
}
#endif

#endif	/* PROTOCOL_H */

