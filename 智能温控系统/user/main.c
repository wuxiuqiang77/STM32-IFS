#include "stm32f10x.h"  // STM32 ��׼�����
#include "OLED.h"       // OLED ��ʾ������
#include "Key.h"        // ��������
#include "DHT11.h"      // DHT11 ��ʪ�ȴ���������
#include "PWM.h"        // PWM ����
#include "Motor.h"      // ���ȵ������
#include "Buzzer.h"     // ����������
#include "Delay.h"      // ��ʱ����

// **ȫ�ֱ���**
uint8_t mode = 0;      // **ģʽѡ��** (0: �Զ�ģʽ, 1: �ֶ�ģʽ)
uint8_t fanLevel = 0;  // **�ֶ�ģʽ�µķ��ȵ�λ** (0: ��, 1: ����, 2: ����)
uint8_t buzzerState = 0;  // **������״̬** (0: �ر�, 1: ����)

void UpdateOLED(int temp, int hum, uint8_t mode, uint8_t fanLevel, uint8_t dhtStatus) {
    OLED_ShowString(1, 1, "Smart Fan V1.0");

    OLED_ShowString(2, 1, "Temp: ");
    if (dhtStatus == 0) {  
        OLED_ShowNum(2, 7, temp, 2);
        OLED_ShowChar(2, 9, 'C');
    } else {
        OLED_ShowString(2, 7, "Err ");
    }

    OLED_ShowString(3, 1, "Hum:  ");
    if (dhtStatus == 0) {
        OLED_ShowNum(3, 7, hum, 2);
        OLED_ShowChar(3, 9, '%');
    } else {
        OLED_ShowString(3, 7, "Err ");
    }

    OLED_ShowString(4, 1, "Mode: ");
    if (mode == 0) {
        OLED_ShowString(4, 7, "Auto   ");
        OLED_ShowString(4, 14, " ");  // **����ֶ�����λ**
    } else {
        OLED_ShowString(4, 7, "Manual ");
        OLED_ShowNum(4, 14, fanLevel, 1);
    }
}


int main(void) {
    int temp = 255, hum = 255;  // **��ʼ����ʪ������**
    uint8_t key, lastKey = 0;   // **��������**
    uint8_t dhtStatus;          // **DHT11 ��ȡ״̬**
    uint16_t buzzerCounter = 0; // **������������**

    // **��ʼ������**
    OLED_Init();
    Key_Init();
    DHT11_Init();
    PWM_Init();
    Motor_Init();
    Buzzer_Init();

    // **��������ʼ����ʾ**
    OLED_Clear();
    UpdateOLED(temp, hum, mode, fanLevel, 1);

    while (1) {
        // **�������**
        key = Key_GetNum();
        if (key != lastKey) {
            Delay_ms(10);  // **����**
            if (key == 1) {  
                mode = !mode;  
                fanLevel = 0;  // **�л��Զ�ģʽʱ������ֶ���**
            } else if (key == 2 && mode == 1) {  
                fanLevel = (fanLevel + 1) % 3;  
            }
            lastKey = key;  // **�洢��ǰ����״̬����ֹ�ظ�����**
        }

        // **DHT11 ��ȡ���ݣ��������Ի��ƣ�**
        uint8_t retry = 3;
        while (retry--) {
            dhtStatus = DHT11_ReadData((uint8_t *)&temp, (uint8_t *)&hum);
            if (dhtStatus == 0) break;
        }
        if (dhtStatus != 0) {
            dhtStatus = 1;  // **�����ȡʧ�ܣ���ˢ�� OLED��������˸**
        }

        // **�Զ�ģʽ**
        if (mode == 0) {
            static uint8_t fanOn = 0;

            if (hum > 70) {
                Motor_SetSpeed(20);
                fanOn = 1;
                buzzerCounter = 0;
            } else if (temp < 70) {
                Motor_SetSpeed(0);
                fanOn = 0;
                buzzerCounter = 0;
            }

            if (fanOn && buzzerCounter < 10) {  
                Buzzer_ON();
                buzzerCounter++;
            } else {
                Buzzer_OFF();
            }
        } 
        // **�ֶ�ģʽ**
        else {  
            if (fanLevel == 0) {
                Motor_SetSpeed(0);
            } else if (fanLevel == 1) {
                Motor_SetSpeed(20);
            } else if (fanLevel == 2) {
                Motor_SetSpeed(50);
            }
            Buzzer_OFF();
            buzzerCounter = 0;
        }

        // **���� OLED ��ʾ**
        UpdateOLED(temp, hum, mode, fanLevel, dhtStatus);
        Delay_ms(400);  // **��С�ӳ٣���߰�����Ӧ**
    }
}

