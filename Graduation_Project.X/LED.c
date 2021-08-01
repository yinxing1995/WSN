#include <xc.h>
#include <string.h>
#include "LED.h"

void LED_Init()
{
    INIT_PIN();
    CONTROL_PIN = 1;
}

void Get_LED(unsigned char * data)
{
    unsigned long value = CONTROL_PIN;
    memcpy(data,&value,sizeof(value));
    return;
}

void ControlLED(unsigned char *command)
{
    if(*command)
    {
        CONTROL_PIN = 1;
    }
    else
    {
        CONTROL_PIN = 0;
    }
}
