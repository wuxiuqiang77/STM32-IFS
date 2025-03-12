#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f10x.h"

#define BUZZER_PORT GPIOB
#define BUZZER_PIN  GPIO_Pin_12

void Buzzer_Init(void);
void Buzzer_ON(void);
void Buzzer_OFF(void);
void Buzzer_Toggle(void);

#endif
