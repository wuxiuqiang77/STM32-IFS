#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"

#define DHT11_PIN    GPIO_Pin_0    // DHT11 数据引脚 (PA0)
#define DHT11_PORT   GPIOA         // DHT11 连接的 GPIO 端口
#define DHT11_CLK    RCC_APB2Periph_GPIOA // DHT11 端口时钟

void DHT11_Init(void);
uint8_t DHT11_ReadData(uint8_t *temperature, uint8_t *humidity);

#endif
