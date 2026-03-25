#include "Inf_DHT11.h"

//全局变量，存储温湿度
uint8_t data[5] = {0};

/**
 * @brief DHT11温湿度传感器初始化函数,需要延迟一秒
 *
 */
void Inf_DHT11_Init(void)
{
    DHT11_DATA_H;
    vTaskDelay(1000);
}

/**
 * @brief 获取DHT11温湿度传感器的温湿度 只保留整数部分
 *
 */
void Inf_DHT11_Get_Temp_Humi(int8_t *temp, int8_t *humi)
{
    int8_t humi_int = 0;
    int8_t temp_int = 0;

    //添加临界区
    //taskENTER_CRITICAL();

    
    // 1.发送起始信号
    DHT11_DATA_L;
    vTaskDelay(20);
    DHT11_DATA_H;

    // 2.等待DHT11的响应
    uint32_t timeout = 0xffffff;//超时退出时间，大约三秒
    while (DHT11_DATA_READ == GPIO_PIN_SET && timeout--)
    {
        // 等待DHT11拉低引脚
    }
    while (DHT11_DATA_READ == GPIO_PIN_RESET && timeout--)
    {
        // 等待DHT11拉高引脚
    }
    while (DHT11_DATA_READ == GPIO_PIN_SET && timeout--)
    {
        // 等待DHT11拉低引脚
    }
    // 3.判断是否超时
    if (timeout == 0)
    {
        DEBUG_PRINT("DHT11响应超时\r\n");
        return;
    }

    taskENTER_CRITICAL();
    //使用双重循环
    for(int i = 0; i < 5; i++)
    {
        data[i] = 0;//前两位是湿度，后两位的温度，最后一位是校验位
        //每一个字节经过八次循环
        for (int j = 0; j < 8; j++)
        {
            // 等待DHT11拉高引脚
            while (DHT11_DATA_READ == GPIO_PIN_RESET)
            {
                // 等待DHT11拉高引脚
            }
            // 高电平27-68us 使用时间长度判断是0还是1
            Inf_Delay_Us(40);
            if (DHT11_DATA_READ == GPIO_PIN_SET)
            {
                // 如果引脚仍然是高电平，说明是1
                data[i] |= (1 << (7 - j));
                //此时还是高电平，需要等待DHT11拉低引脚
                while (DHT11_DATA_READ == GPIO_PIN_SET)
                {
                    // 等待DHT11拉低引脚
                }
            }
            else
            {
                // 如果引脚是低电平，说明是0
            }
        }
    }

    // 4.校验数据
    uint32_t sum = data[0] + data[1] + data[2] + data[3];
    if ((uint8_t)sum == data[4])
    {
        humi_int = data[0];
        temp_int = data[2];
        if(data[3] & 0x80)
        {
            temp_int = -temp_int;
        }

        //赋值返回
        *humi = humi_int;
        *temp = temp_int;
        
    }
    else
    {
        DEBUG_PRINT("DHT11数据校验失败\r\n");
    }
    taskEXIT_CRITICAL();
}
