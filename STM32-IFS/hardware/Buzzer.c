#include "Buzzer.h"

/**
  * @brief  ��ʼ�������� (PB12)
  * @param  ��
  * @retval ��
  */
void Buzzer_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // ʹ��GPIOBʱ��

    GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);

    Buzzer_OFF(); // Ĭ�Ϲرշ�����
}

/**
  * @brief  �򿪷�����
  * @param  ��
  * @retval ��
  */
void Buzzer_ON(void)
{
    GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN);
}

/**
  * @brief  �رշ�����
  * @param  ��
  * @retval ��
  */
void Buzzer_OFF(void)
{
    GPIO_SetBits(BUZZER_PORT, BUZZER_PIN);
}

/**
  * @brief  �л�������״̬ (��/��)
  * @param  ��
  * @retval ��
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
