#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

// LED 宏定义
#define LED1_PIN        GPIO_Pin_13
#define LED2_PIN        GPIO_Pin_14
#define LED_PORT        GPIOB

// LED 控制函数声明
void LED_Init(void);
void LED1_ON(void);
void LED1_OFF(void);
void LED1_Toggle(void);
void LED2_ON(void);
void LED2_OFF(void);
void LED2_Toggle(void);

#endif
