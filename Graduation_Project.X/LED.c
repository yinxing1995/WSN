#include <xc.h>
#include <string.h>
#include "LED.h"
#include "io.h"

static unsigned long status = 0;
void LED_Init()
{
    INIT_PIN();
    CONTROL_PIN = 1;
    status = 1;
}

void Get_LED(void * data)
{
    unsigned long value = status;
    memcpy(data,&value,sizeof(value));
    Uprintf("Get %d",status);
    return;
}

void ControlLED(unsigned char *command)
{
    unsigned long onoff;
    onoff = (unsigned long *)command;
    if(*command)
    {
        CONTROL_PIN = 1;
        status = 1;
    }
    else
    {
        CONTROL_PIN = 0;
        status = 0;
    }
}
