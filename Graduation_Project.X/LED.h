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

#define INIT_PIN()  TRISAbits.RA0 = 0;
#define CONTROL_PIN PORTAbits.RA0

unsigned long OnOffStatus;//On 1 | Off 0

void LED_Init();
void Get_LED(unsigned char *);
void ControlLED(unsigned char *);

#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */

