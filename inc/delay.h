/*
 * delay.h
 *
 *  Created on: Apr 17, 2017
 *      Author: nguyenbui
 */

#ifndef DELAY_H_
#define DELAY_H_
#include "stm32f4xx.h"
#include "stdbool.h" //for bool

volatile u32 Millis;
volatile u32 Micros;
u32 micros(void);
u32 millis(void);

void Systick_Configuration(void);//initialize systick
void delay_ms(u32 nTime);
void delay_us(u32 nTime);

void elapseMicros(u32 targetTime, u32 oldt);
void elapseMillis(u32 targetTime, u32 oldt);

#define useExternalOSC 0     //true=1/false=0
#define systemFrequency 168   //MHz


#endif /* DELAY_H_ */
