/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2026-03-17 13:03:59
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2026-03-21 10:39:59
 * @FilePath: \MDK-ARM\interface\Inf_led.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "Inf_led.h"

// 设计数组保存数字
// 时针的个位和十位，根据原理图led灯管接口计算得到
uint16_t led_h_s[3] = {0x0000, 0x0006, 0x005b};
uint16_t led_h_g[10] = {0x1f80, 0x0300, 0x2d80, 0x2780, 0x3300, 0x3680, 0x3e80, 0x0380, 0x3f80, 0x3780};
// 分针的各位和十位
uint16_t led_m_s[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
uint16_t led_m_g[10] = {0x1f80, 0x0300, 0x2d80, 0x2780, 0x3300, 0x3680, 0x3e80, 0x0380, 0x3f80, 0x3780};

// 温湿度数字数组
uint16_t led_tm_s[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
uint16_t led_tm_g[10] = {0x1f80, 0x0300, 0x2d80, 0x2780, 0x3300, 0x3680, 0x3e80, 0x0380, 0x3f80, 0x3780};

uint16_t led_hu_s[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
uint16_t led_hu_g[10] = {0x1f80, 0x0300, 0x2d80, 0x2780, 0x3300, 0x3680, 0x3e80, 0x0380, 0x3f80, 0x3780};

/**
 * @brief LED初始化，P04引脚开启，两个led驱动芯片的供电开启
 *
 */
void Inf_LED_Start(void)
{
    HAL_GPIO_WritePin(P04_GPIO_Port, P04_Pin, GPIO_PIN_RESET);
}

/**
 * @brief LED停止
 *
 */
void Inf_LED_Stop(void)
{
    LED_OEA_H;
    LED_OEB_H;  
}

/**
 * @brief LED写数据
 *
 * @param P00 选择电源引脚P00，0或1
 * @param P01 选择电源引脚P01，0或1
 * @param P02 选择电源引脚P02，0或1
 * @param data 要写入的数据，32位无符号整数
 */
void Inf_LED_Write(uint8_t P00, uint8_t P01, uint8_t P02, uint32_t data)
{

    // 写入数据 高位在前
    for (uint8_t i = 0; i < 32; i++)
    {
        if (data & (0x80000000 >> i)) // 判断最高位
        {
            LED_SDI_H;
        }
        else
        {
            LED_SDI_L;
        }
        LED_CLK_H;
        LED_CLK_L;
    }
    // 锁存数据
    LED_LE_H;
    LED_LE_L;

    // 选择电源引脚，所有led的阳极都接在vccp00-p02，阴极都接在led驱动芯片的输出口，通过stm32向驱动芯片发送数据，来控制led的亮灭
    //两个led驱动芯片，left和right是串行连接，一个芯片驱动16个led，两个驱动最多32个led，right是低十六位，left是高十六位。先发送的十六位数据到left，再发送十六位数据，原先的十六位数据由left发送到riht，后来的十六位数据到再left芯片
    //故把所有的led灯管分为三组，分别接在VCCP00，VCCP01和VCCP02
    HAL_GPIO_WritePin(VCCP00_GPIO_Port, VCCP00_Pin, (GPIO_PinState)P00);
    HAL_GPIO_WritePin(VCCP01_GPIO_Port, VCCP01_Pin, (GPIO_PinState)P01);
    HAL_GPIO_WritePin(VCCP02_GPIO_Port, VCCP02_Pin, (GPIO_PinState)P02);

    // 输出使能
    LED_OEA_L;
    LED_OEB_L;
}

/**
 * @brief 设置时间
 */
void Inf_LED_Set_Clock(uint8_t hour, uint8_t minute, uint8_t clock_flag)
{
    // 时针亮5ms
    uint16_t H_shi = hour / 10;
    uint16_t H_ge = hour % 10;
    uint32_t data;
    if (clock_flag)
    {
        data = 0x20 + led_h_s[H_shi] + led_h_g[H_ge];
    }
    else
    {
        data = led_h_s[H_shi] + led_h_g[H_ge];
    }
    Inf_LED_Write(0, 1, 1, data);
    vTaskDelay(5);

    // 分针亮5ms
    uint16_t M_shi = minute / 10;
    uint16_t M_ge = minute % 10;
    Inf_LED_Write(1, 0, 1, led_m_s[M_shi] + led_m_g[M_ge]);
    vTaskDelay(5);
}

// 控制温湿度、时间、和其他的led亮灭
//八位的寄存器，高四位存储十位，第四位存储个位：data / 10是取十位的数据，data % 10 取个位的数据，两者相加是完整数据
//参数是task1已经采集好的温湿度和时间，以及展示状态的结构体
//通过data参数依次改变，且VCC电源同步改变，5ms改变一次，达到人眼暂留效果
void Inf_led_set_all(uint8_t hour, uint8_t minute, uint8_t flag, int8_t humidity, int8_t tempreture, Show_Type_Struct *Show_Type)
{
    //三根led接在VCCP02，常亮，存储温度的正负，是否超过100度，
    uint32_t VCCP02_data = 0x38;

    //高十六位的数据，在VCCP00时存储湿度的个位和十位
    //0x4000 : 0100 0000 0000 0000 即高十六位中的第十五根led，常亮
    uint16_t data_right = led_hu_s[humidity / 10] + led_hu_g[humidity % 10] + 0x4000;
    
    //低十六位数据 在VCCP00时存储时间的小时和分钟，以及一些标志位
    uint16_t data_left = 0;

    uint16_t am_pm_data = 0;
    // 12/24小时标识
    if (Show_Type->is_12_hour)
    {
        if (Show_Type->is_pm)
        {
            am_pm_data = 0x8000;
        }
        else
        {
            am_pm_data = 0x4000;
        }
    }
    //时钟和分钟之间的两点 ：标识 0x20，一秒闪烁一次 
    if (flag)
    {
        // 添加逻辑 hour > 99 ，表示不展示
        if(hour > 99)
        {
            data_left = am_pm_data + 0x20;
        }
        else
        {
            data_left = led_h_s[hour / 10] + led_h_g[hour % 10] + am_pm_data + 0x20;
            //0x20: 0010 0000,即第六根led
        }
    }
    else
    {
        if(hour > 99) //目的是为了后面的时间设置界面，时间设置界面需要闪烁小时或者分钟，如果小时或者分钟大于99，就不展示这个数字，只展示12/24小时的标识和冒号
        {
            data_left = am_pm_data;
        }
        else
        {
            data_left = led_h_s[hour / 10] + led_h_g[hour % 10] + am_pm_data;
        }
        
    }

    // 五天闹钟标识
    if (Show_Type->is_alarm_five)
    {
        data_right += 0x8000;
    }

    Inf_LED_Write(0, 1, 1, (data_right << 16 | data_left));
    //由于是高位先行，所以左移十六位，保证能到达right芯片
    vTaskDelay(5);

    // 摄氏度华氏度标识
    //修改温度的值
    int8_t Temp_now = tempreture;
    uint16_t tempreture_data = 0;
    if (Show_Type->is_celsius)
    {
        tempreture_data = 0x4000;
    }
    else
    {
        tempreture_data = 0x8000;
        Temp_now = (tempreture * 9 / 5) + 32;
    }

    // 拼接温度，只保留个位和十位
    if (Temp_now < 0)
    {
        // 负号标识
        Temp_now = -Temp_now;
        VCCP02_data += 0x02;
    }
    if (Temp_now > 99)
    {
        // 百位标识
        Temp_now -= 100;
        VCCP02_data += 0x05;
    }

    //此时的高十六位数据存储温度个位和十位，以及摄氏度华氏度标识符
    data_right = led_tm_s[Temp_now / 10] + led_tm_g[Temp_now % 10] + tempreture_data;


    if(minute > 99)
    {
        data_left = 0;
    }
    else
    {
        data_left = led_m_s[minute / 10] + led_m_g[minute % 10];        
    }

    // 闹钟LED
    uint16_t Alarm_data = 0;
    if (Show_Type->is_alarm_1)
    {
        Alarm_data += 0x4000;
    }
    if (Show_Type->is_alarm_2)
    {
        Alarm_data += 0x8000;
    }
    //此时的低十六位数据存储时间分钟的个位和十位，以及闹钟1，2的标志位
    data_left = data_left + Alarm_data;

    Inf_LED_Write(1, 0, 1, (data_right << 16 | data_left));
    vTaskDelay(5);

    Inf_LED_Write(1, 1, 0, (VCCP02_data << 16));
    vTaskDelay(5);

    // 特殊灯标识 常亮
    if (Show_Type->is_charging)
    {
        HAL_GPIO_WritePin(P03_GPIO_Port, P03_Pin, GPIO_PIN_RESET);
    }
}
    //总结流程：Inf_led_set_all将采集数据汇总，stm32发送数据给led驱动芯片，同时控制三组灯管组的VCC，达到同时控制所有的led亮灭的效果

    //三组VCC灯管，每组灯管代表的意义不同，VCCP00主要控制时间的显示，VCCP01主要控制温湿度的显示，VCCP02主要控制一些特殊标识灯的显示（温度的正负，是否超过100度，五天闹钟，等等），通过轮流控制三组VCC（ vTaskDelay(5);），可以达到近似同时控制所有灯管的效果，同时也满足了人眼的暂留效果



//num1 > 99 不亮 ，num2 > 99 不亮 ， point ：1 亮 ， 0 不亮
void Inf_led_time_set_num(uint16_t num1,uint16_t num2,uint8_t point)
{
    uint16_t vccp00_data = 0;
    uint16_t vccp01_data = 0;
    uint16_t point_data = 0;
    if(point)
    {
        point_data = 0x20;
    }

    //根据第一个数值计算vccp00的值
    if(num1 > 99)
    {
        vccp00_data = point_data + 0;
    }
    else
    {
        vccp00_data = led_h_s[num1 / 10] + led_h_g[num1 % 10] + point_data;
    }

    if(num2 > 99)
    {
        vccp01_data = 0x00;
    }
    else
    {
        vccp01_data = led_m_s[num2 / 10] + led_m_g[num2 % 10];
    }

    //两次时间点亮
    Inf_LED_Write(0,1,1,vccp00_data);
    vTaskDelay(5);
    Inf_LED_Write(1,0,1,vccp01_data);
    vTaskDelay(5);
    
}

uint32_t time_set_last_time = 0;
uint8_t led_flag= 0;

//时间设置的led页面显示
void Inf_led_time_set(Show_Time_Set time_set,uint16_t value)
{
    //闪烁功能实现
    if(xTaskGetTickCount() - time_set_last_time >= 500)
    {
        led_flag =! led_flag;

        time_set_last_time = xTaskGetTickCount();
    }

    switch (time_set)
    {
    case Time_Set_Year:
        if(led_flag)
        {
            Inf_led_time_set_num(value / 100, value % 100, 0);
        }
        else
        {
            Inf_led_time_set_num(value / 100, 0xff, 0);
            //ff表示一个极大值，后两位不亮
        }
        //前两位显示20，后两位闪烁，可修改
        break;

    case Time_Set_Month:
        if(led_flag)
        {
            Inf_led_time_set_num(value, 0xff, 0);
        }
        else
        {
            Inf_led_time_set_num(0xff, 0xff, 0);
        }
        //前两位闪烁，后两位熄灭，可修改
        break;

    case Time_Set_Day:
        if(led_flag)
        {
            Inf_led_time_set_num(0xff, value, 0);
        }
        else
        {
            Inf_led_time_set_num(0xff, 0xff, 0);
        }
        break;

    case Time_Set_Hour:
        if(led_flag)
        {
            Inf_led_time_set_num(value, 0xff, 1);
            //把点打开，区别与年月日
        }
        else
        {
            Inf_led_time_set_num(0xff, 0xff, 1);
        }
        break;

    case Time_Set_Min:
        if(led_flag)
        {
            Inf_led_time_set_num(0xff, value, 1);
            //把点打开，区别与年月日
        }
        else
        {
            Inf_led_time_set_num(0xff, 0xff, 1);
        }
        break;
    
    default:
        break;
    }
}
