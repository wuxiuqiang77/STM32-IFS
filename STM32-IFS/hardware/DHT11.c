#include "DHT11.h"
#include "Delay.h"

// ���� GPIO Ϊ���ģʽ
static void DHT11_SetPinOutput(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

// ���� GPIO Ϊ����ģʽ���������룩
static void DHT11_SetPinInput(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // ��������ģʽ
    GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

// ��ʼ�� DHT11
void DHT11_Init(void) {
    RCC_APB2PeriphClockCmd(DHT11_CLK, ENABLE); // ȷ�� GPIO ʱ�ӿ���
    DHT11_SetPinOutput();
    GPIO_SetBits(DHT11_PORT, DHT11_PIN); // ��ʼ״̬����
}

// ��ȡ DHT11 ����
uint8_t DHT11_ReadData(uint8_t *temperature, uint8_t *humidity) {
    uint8_t data[5] = {0, 0, 0, 0, 0};
    uint8_t i, j;
    uint32_t timeout = 0;

    // 1. MCU ������ʼ�ź�
    DHT11_SetPinOutput();
    GPIO_ResetBits(DHT11_PORT, DHT11_PIN); // �������� 18ms
    Delay_ms(18);
    GPIO_SetBits(DHT11_PORT, DHT11_PIN);   // �ͷ�����
    Delay_us(30);

    // 2. �ȴ� DHT11 ��Ӧ
    DHT11_SetPinInput();

    timeout = 10000;
    while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) { // �ȴ�����
        if (--timeout == 0) return 1; // ����������Ӧ
    }

    timeout = 10000;
    while (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) { // �͵�ƽ 80us
        if (--timeout == 0) return 1;
    }

    timeout = 10000;
    while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) { // �ߵ�ƽ 80us
        if (--timeout == 0) return 1;
    }

    // 3. ��ȡ 40 λ����
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 8; j++) {
            timeout = 10000;
            while (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) { // �ȴ��ߵ�ƽ
                if (--timeout == 0) return 1;
            }

            Delay_us(50); // **����ʱ������ 0 �� 1**

            if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
                data[i] |= (1 << (7 - j)); // ��ȡ bit1

                timeout = 10000;
                while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) { // �ȴ��͵�ƽ
                    if (--timeout == 0) return 1;
                }
            }
        }
    }

    // 4. ����У��
    if ((data[0] + data[1] + data[2] + data[3]) != data[4]) return 2; // У�����

    // 5. ��������
    *humidity = data[0];      // ʪ����������
    *temperature = data[2];   // �¶���������

    return 0; // ��ȡ�ɹ�
}
