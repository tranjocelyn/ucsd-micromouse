/*
 * led.h
 *
 *  Created on: Apr 17, 2017
 *      Author: nguyenbui
 */

#ifndef LED_H_
#define LED_H_

/*
 * Pins configuration for Basic mouse:
 * 	Left Diagonal(LD) TX --> Pin PC3
 * 	Low Front (FL) TX --> Pin PC1
 *  High Front (FH) TX --> Pin PC2
 *  Right Diagonal (RD) TX --> Pin PC0
 */

/*
 * Pins configuration for Basic mouse:
 * 	Left Diagonal(LD) TX --> Pin PC3
 * 	Low Front (FL) TX --> Pin PC1
 *  High Front (FH) TX --> Pin PC2
 *  Right Diagonal (RD) TX --> Pin PC0
 */

#define LED1_OFF    GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define LED1_ON   GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define LED2_OFF    GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define LED2_ON   GPIO_ResetBits(GPIOB, GPIO_Pin_13)
#define LED3_OFF    GPIO_SetBits(GPIOB, GPIO_Pin_14)
#define LED3_ON   GPIO_ResetBits(GPIOB, GPIO_Pin_14)
#define LED4_OFF    GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define LED4_ON   GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define LED5_OFF    GPIO_SetBits(GPIOC, GPIO_Pin_14)
#define LED5_ON   GPIO_ResetBits(GPIOC, GPIO_Pin_14)
#define LED6_OFF    GPIO_SetBits(GPIOC, GPIO_Pin_15)
#define LED6_ON   GPIO_ResetBits(GPIOC, GPIO_Pin_15)


#define FH_EM_ON   GPIO_SetBits(GPIOC, GPIO_Pin_1); \
                   GPIO_SetBits(GPIOC, GPIO_Pin_0)
#define FH_EM_OFF  GPIO_ResetBits(GPIOC, GPIO_Pin_1); \
                   GPIO_ResetBits(GPIOC, GPIO_Pin_0)
#define FL_EM_ON   GPIO_SetBits(GPIOC, GPIO_Pin_0)
#define FL_EM_OFF  GPIO_ResetBits(GPIOC, GPIO_Pin_0)
#define LD_EM_ON   GPIO_SetBits(GPIOC, GPIO_Pin_3)
#define LD_EM_OFF  GPIO_ResetBits(GPIOC, GPIO_Pin_3)
#define RD_EM_ON   GPIO_SetBits(GPIOC, GPIO_Pin_2)
#define RD_EM_OFF  GPIO_ResetBits(GPIOC, GPIO_Pin_2)

#define ALL_LED_OFF LED1_OFF; \
                    LED2_OFF; \
                    LED3_OFF; \
					LED4_OFF; \
					LED5_OFF; \
					LED6_OFF


#define ALL_LED_ON  LED1_ON; \
                    LED2_ON; \
					LED3_ON; \
					LED4_ON; \
					LED5_ON; \
					LED6_ON

void LED_Configuration(void);


#endif /* LED_H_ */
