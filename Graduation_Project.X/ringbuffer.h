#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>

typedef struct buffer
{
    volatile uint8_t *position;
    volatile uint16_t writepointer;
    volatile uint16_t readpointer;
    volatile uint16_t max;
}Ringbuf;

volatile Ringbuf Buffer;

void BufferInit(char *p, uint16_t size);
int BufferWrite(void *sour, uint16_t len);
int BufferRead(void *dest, uint16_t len);
int BufferSeek(void *dest, uint16_t len);

#endif
