/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2026-03-17 13:04:13
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2026-03-19 15:11:35
 * @FilePath: \MDK-ARM\interface\Inf_led.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __INF_LED__
#define __INF_LED__

#include "gpio.h"
#include "freertos.h"
#include "task.h"

extern uint16_t led_h_s[3];
extern uint16_t led_h_g[10];

extern uint16_t led_m_s[10];
extern uint16_t led_m_g[10];

#define LED_CLK_H HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, GPIO_PIN_SET)
#define LED_CLK_L HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, GPIO_PIN_RESET)

#define LED_SDI_H HAL_GPIO_WritePin(LED_SDI_GPIO_Port, LED_SDI_Pin, GPIO_PIN_SET)
#define LED_SDI_L HAL_GPIO_WritePin(LED_SDI_GPIO_Port, LED_SDI_Pin, GPIO_PIN_RESET)

#define LED_LE_H HAL_GPIO_WritePin(LED_LE_GPIO_Port, LED_LE_Pin, GPIO_PIN_SET)
#define LED_LE_L HAL_GPIO_WritePin(LED_LE_GPIO_Port, LED_LE_Pin, GPIO_PIN_RESET)

#define LED_OEA_H HAL_GPIO_WritePin(LED_OEA_GPIO_Port, LED_OEA_Pin, GPIO_PIN_SET)
#define LED_OEA_L HAL_GPIO_WritePin(LED_OEA_GPIO_Port, LED_OEA_Pin, GPIO_PIN_RESET)

#define LED_OEB_H HAL_GPIO_WritePin(LED_OEB_GPIO_Port, LED_OEB_Pin, GPIO_PIN_SET)
#define LED_OEB_L HAL_GPIO_WritePin(LED_OEB_GPIO_Port, LED_OEB_Pin, GPIO_PIN_RESET)

typedef struct 
{
    uint8_t is_12_hour;
    uint8_t is_pm;
    uint8_t is_celsius;
    uint8_t is_alarm_1;
    uint8_t is_alarm_2;
    uint8_t is_alarm_five;
    uint8_t is_charging;
}Show_Type_Struct;

typedef enum
{
    Time_Set_Year = 0,
    Time_Set_Month,
    Time_Set_Day,
    Time_Set_Hour,
    Time_Set_Min,
    Time_Set_End,
}Show_Time_Set;

typedef enum
{
    Alarm_Set_Hour = 0,
    Alarm_Set_Minute,
    Alarm_Set_End,
}Show_alarm_set;


/**
 * @brief LED初始化
 *
 */
void Inf_LED_Start(void);

/**
 * @brief LED停止
 *
 */
void Inf_LED_Stop(void);

/**
 * @brief LED写数据
 *
 * @param P00 选择电源引脚P00，0或1
 * @param P01 选择电源引脚P01，0或1
 * @param P02 选择电源引脚P02，0或1
 * @param data 要写入的数据，32位无符号整数
 */
void Inf_LED_Write(uint8_t P00,uint8_t P01,uint8_t P02,uint32_t data);


void Inf_led_time_set_num(uint16_t num1,uint16_t num2,uint8_t point);

/**
 * @brief 设置时间
 */
void Inf_LED_Set_Clock(uint8_t hour,uint8_t minute,uint8_t clock_flag);

void Inf_led_set_all(uint8_t hour,uint8_t minute,uint8_t flag, int8_t humidity , int8_t tempreture, Show_Type_Struct *Show_Type);


//时间设置页面显示
void Inf_led_time_set(Show_Time_Set time_set,uint16_t value);


#endif
