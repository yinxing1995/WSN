/* 
 * File:   LED.h
 * Author: asus
 *
 * Created on 2021?7?31?, ??11:49
 */

#ifndef LED_H
#define	LED_H

#ifdef	__cplusplus
extern "C" {
#endif

#define INIT_PIN()  TRISAbits.RA1 = 0;\
                    TRISAbits.RA3 = 1;
    
#define CONTROL_PIN PORTAbits.RA1
//#define READ_PIN PORTAbits.RA3

unsigned long OnOffStatus;//On 1 | Off 0

void LED_Init();
void Get_LED(void *);
void ControlLED(unsigned char *);

#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */

