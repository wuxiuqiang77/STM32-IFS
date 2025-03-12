#include "led.h"
#include "stm32f10x.h"                  // Device header


/**
  * @brief  初始化 LED (PB13 和 PB14)
  * @param  无
  * @retval 无
  */
void LED_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能 GPIOB 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 配置 GPIO 模式为推挽输出
    GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    // 默认关闭 LED (高电平关闭，假设 LED 共阳接法)
    GPIO_SetBits(LED_PORT, LED1_PIN | LED2_PIN);
}

/**
  * @brief  打开 LED1 (PB13)
  * @param  无
  * @retval 无
  */
void LED1_ON(void) {
    GPIO_ResetBits(LED_PORT, LED1_PIN); // 低电平点亮（假设共阳接法）
}

/**
  * @brief  关闭 LED1 (PB13)
  * @param  无
  * @retval 无
  */
void LED1_OFF(void) {
    GPIO_SetBits(LED_PORT, LED1_PIN); // 高电平熄灭
}

/**
  * @brief  切换 LED1 (PB13) 状态
  * @param  无
  * @retval 无
  */
void LED1_Toggle(void) {
    if (GPIO_ReadOutputDataBit(LED_PORT, LED1_PIN) == Bit_SET) {
        LED1_ON();
    } else {
        LED1_OFF();
    }
}

/**
  * @brief  打开 LED2 (PB14)
  * @param  无
  * @retval 无
  */
void LED2_ON(void) {
    GPIO_ResetBits(LED_PORT, LED2_PIN); // 低电平点亮
}

/**
  * @brief  关闭 LED2 (PB14)
  * @param  无
  * @retval 无
  */
void LED2_OFF(void) {
    GPIO_SetBits(LED_PORT, LED2_PIN); // 高电平熄灭
}

/**
  * @brief  切换 LED2 (PB14) 状态
  * @param  无
  * @retval 无
  */
void LED2_Toggle(void) {
    if (GPIO_ReadOutputDataBit(LED_PORT, LED2_PIN) == Bit_SET) {
        LED2_ON();
    } else {
        LED2_OFF();
    }
}
