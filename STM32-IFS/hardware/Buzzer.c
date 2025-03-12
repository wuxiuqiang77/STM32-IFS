#include "Buzzer.h"

/**
  * @brief  初始化蜂鸣器 (PB12)
  * @param  无
  * @retval 无
  */
void Buzzer_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能GPIOB时钟

    GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);

    Buzzer_OFF(); // 默认关闭蜂鸣器
}

/**
  * @brief  打开蜂鸣器
  * @param  无
  * @retval 无
  */
void Buzzer_ON(void)
{
    GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN);
}

/**
  * @brief  关闭蜂鸣器
  * @param  无
  * @retval 无
  */
void Buzzer_OFF(void)
{
    GPIO_SetBits(BUZZER_PORT, BUZZER_PIN);
}

/**
  * @brief  切换蜂鸣器状态 (开/关)
  * @param  无
  * @retval 无
  */
void Buzzer_Toggle(void)
{
    if (GPIO_ReadOutputDataBit(BUZZER_PORT, BUZZER_PIN))
    {
        Buzzer_ON();
    }
    else
    {
        Buzzer_OFF();
    }
}
