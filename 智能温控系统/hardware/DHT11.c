#include "DHT11.h"
#include "Delay.h"

// 设置 GPIO 为输出模式
static void DHT11_SetPinOutput(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

// 设置 GPIO 为输入模式（上拉输入）
static void DHT11_SetPinInput(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入模式
    GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

// 初始化 DHT11
void DHT11_Init(void) {
    RCC_APB2PeriphClockCmd(DHT11_CLK, ENABLE); // 确保 GPIO 时钟开启
    DHT11_SetPinOutput();
    GPIO_SetBits(DHT11_PORT, DHT11_PIN); // 初始状态拉高
}

// 读取 DHT11 数据
uint8_t DHT11_ReadData(uint8_t *temperature, uint8_t *humidity) {
    uint8_t data[5] = {0, 0, 0, 0, 0};
    uint8_t i, j;
    uint32_t timeout = 0;

    // 1. MCU 发送起始信号
    DHT11_SetPinOutput();
    GPIO_ResetBits(DHT11_PORT, DHT11_PIN); // 拉低至少 18ms
    Delay_ms(18);
    GPIO_SetBits(DHT11_PORT, DHT11_PIN);   // 释放总线
    Delay_us(30);

    // 2. 等待 DHT11 响应
    DHT11_SetPinInput();

    timeout = 10000;
    while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) { // 等待拉低
        if (--timeout == 0) return 1; // 传感器无响应
    }

    timeout = 10000;
    while (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) { // 低电平 80us
        if (--timeout == 0) return 1;
    }

    timeout = 10000;
    while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) { // 高电平 80us
        if (--timeout == 0) return 1;
    }

    // 3. 读取 40 位数据
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 8; j++) {
            timeout = 10000;
            while (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) { // 等待高电平
                if (--timeout == 0) return 1;
            }

            Delay_us(50); // **调整时序，区分 0 和 1**

            if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
                data[i] |= (1 << (7 - j)); // 读取 bit1

                timeout = 10000;
                while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) { // 等待低电平
                    if (--timeout == 0) return 1;
                }
            }
        }
    }

    // 4. 数据校验
    if ((data[0] + data[1] + data[2] + data[3]) != data[4]) return 2; // 校验错误

    // 5. 解析数据
    *humidity = data[0];      // 湿度整数部分
    *temperature = data[2];   // 温度整数部分

    return 0; // 读取成功
}
