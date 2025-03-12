#include "led.h"
#include "stm32f10x.h"                  // Device header


/**
  * @brief  ��ʼ�� LED (PB13 �� PB14)
  * @param  ��
  * @retval ��
  */
void LED_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // ʹ�� GPIOB ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // ���� GPIO ģʽΪ�������
    GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    // Ĭ�Ϲر� LED (�ߵ�ƽ�رգ����� LED �����ӷ�)
    GPIO_SetBits(LED_PORT, LED1_PIN | LED2_PIN);
}

/**
  * @brief  �� LED1 (PB13)
  * @param  ��
  * @retval ��
  */
void LED1_ON(void) {
    GPIO_ResetBits(LED_PORT, LED1_PIN); // �͵�ƽ���������蹲���ӷ���
}

/**
  * @brief  �ر� LED1 (PB13)
  * @param  ��
  * @retval ��
  */
void LED1_OFF(void) {
    GPIO_SetBits(LED_PORT, LED1_PIN); // �ߵ�ƽϨ��
}

/**
  * @brief  �л� LED1 (PB13) ״̬
  * @param  ��
  * @retval ��
  */
void LED1_Toggle(void) {
    if (GPIO_ReadOutputDataBit(LED_PORT, LED1_PIN) == Bit_SET) {
        LED1_ON();
    } else {
        LED1_OFF();
    }
}

/**
  * @brief  �� LED2 (PB14)
  * @param  ��
  * @retval ��
  */
void LED2_ON(void) {
    GPIO_ResetBits(LED_PORT, LED2_PIN); // �͵�ƽ����
}

/**
  * @brief  �ر� LED2 (PB14)
  * @param  ��
  * @retval ��
  */
void LED2_OFF(void) {
    GPIO_SetBits(LED_PORT, LED2_PIN); // �ߵ�ƽϨ��
}

/**
  * @brief  �л� LED2 (PB14) ״̬
  * @param  ��
  * @retval ��
  */
void LED2_Toggle(void) {
    if (GPIO_ReadOutputDataBit(LED_PORT, LED2_PIN) == Bit_SET) {
        LED2_ON();
    } else {
        LED2_OFF();
    }
}
