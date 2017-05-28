/*
 * encoder.c
 *
 *  Created on: Apr 17, 2017
 *      Author: nguyenbui
 */

#include "stm32f4xx.h"


volatile int32_t global_left_dist = 0;
volatile int32_t global_right_dist = 0;
volatile int32_t global_left_speed = 0;
volatile int32_t global_right_speed = 0;

//PA0	TIM5_CH1	Encoder_R_CHA
//PA1	TIM5_CH2	Encoder_R_CHB
//PA15	TIM2_CH1	Encoder_L_CHA
//PB3	TIM2_CH2	Encoder_L_CHB

void setLeftEncCount(int32_t cnt) {
	TIM3->CNT = (int16_t)cnt;
}

void setRightEncCount(int32_t cnt) {
	TIM5->CNT = cnt;
}

# warning Check board configuration
/*
 * This code is use for the advantage mice. with is using TIM3 instead of TIM2
 *
 * For basic mice, change everything from 2 to 3
 * GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_15;
 * GPIO_Init(GPIOA, &GPIO_InitStructure);
 * GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
 * GPIO_Init(GPIOB, &GPIO_InitStructure);
 *
 *
 * GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
 * GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
 * GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
 * GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);
 */
void Encoder_Configration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5|RCC_APB1Periph_TIM3, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);

  TIM_SetAutoreload (TIM5, 0xffffffff);//0xffffffff is the max value for 32 bit, the autoreload value will be 0xffff for 16 bit timer
	TIM_SetAutoreload (TIM3, 0xffff);
	/* Configure the encoder */
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);//rising rising or rising falling will help you to swithc the direction for encoder at quardrature mode
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//if the setting is rising rising and the encoder counts decreases when wheel spin forward, just change it to rising falling
	/* TIM4 counter enable */
	TIM_Cmd(TIM5, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	// Reset both register
	setLeftEncCount(0);
	setRightEncCount(0);
}

int32_t getLeftDistance(void){
	// CNT is uint32_t
	// Manually cast from signed 16bit to signed 32bit
	return (global_left_dist + (int16_t)TIM3->CNT);
}
int32_t getRightDistance(void){
	return (global_right_dist + TIM5->CNT);
}

void update_speed(void){
	// CNT is uint32_t
	// Manually cast from signed 16bit to signed 32bit
	global_left_speed = (int16_t)TIM3->CNT;
	TIM3->CNT = 0;
	global_right_speed = TIM5->CNT;
	TIM5->CNT = 0;
	global_left_dist += global_left_speed;
	global_right_dist += global_right_speed;
}
