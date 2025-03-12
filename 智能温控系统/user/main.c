#include "stm32f10x.h"  // STM32 标准外设库
#include "OLED.h"       // OLED 显示屏驱动
#include "Key.h"        // 按键驱动
#include "DHT11.h"      // DHT11 温湿度传感器驱动
#include "PWM.h"        // PWM 控制
#include "Motor.h"      // 风扇电机驱动
#include "Buzzer.h"     // 蜂鸣器驱动
#include "Delay.h"      // 延时函数

// **全局变量**
uint8_t mode = 0;      // **模式选择** (0: 自动模式, 1: 手动模式)
uint8_t fanLevel = 0;  // **手动模式下的风扇档位** (0: 关, 1: 低速, 2: 高速)
uint8_t buzzerState = 0;  // **蜂鸣器状态** (0: 关闭, 1: 开启)

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
        OLED_ShowString(4, 14, " ");  // **清除手动挡挡位**
    } else {
        OLED_ShowString(4, 7, "Manual ");
        OLED_ShowNum(4, 14, fanLevel, 1);
    }
}


int main(void) {
    int temp = 255, hum = 255;  // **初始化温湿度数据**
    uint8_t key, lastKey = 0;   // **按键变量**
    uint8_t dhtStatus;          // **DHT11 读取状态**
    uint16_t buzzerCounter = 0; // **蜂鸣器计数器**

    // **初始化外设**
    OLED_Init();
    Key_Init();
    DHT11_Init();
    PWM_Init();
    Motor_Init();
    Buzzer_Init();

    // **清屏并初始化显示**
    OLED_Clear();
    UpdateOLED(temp, hum, mode, fanLevel, 1);

    while (1) {
        // **按键检测**
        key = Key_GetNum();
        if (key != lastKey) {
            Delay_ms(10);  // **消抖**
            if (key == 1) {  
                mode = !mode;  
                fanLevel = 0;  // **切换自动模式时，清除手动挡**
            } else if (key == 2 && mode == 1) {  
                fanLevel = (fanLevel + 1) % 3;  
            }
            lastKey = key;  // **存储当前按键状态，防止重复触发**
        }

        // **DHT11 读取数据（增加重试机制）**
        uint8_t retry = 3;
        while (retry--) {
            dhtStatus = DHT11_ReadData((uint8_t *)&temp, (uint8_t *)&hum);
            if (dhtStatus == 0) break;
        }
        if (dhtStatus != 0) {
            dhtStatus = 1;  // **如果读取失败，则不刷新 OLED，避免闪烁**
        }

        // **自动模式**
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
        // **手动模式**
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

        // **更新 OLED 显示**
        UpdateOLED(temp, hum, mode, fanLevel, dhtStatus);
        Delay_ms(400);  // **减小延迟，提高按键响应**
    }
}

