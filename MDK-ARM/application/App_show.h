/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2026-03-17 20:36:07
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2026-03-23 10:02:53
 * @FilePath: \MDK-ARM\application\App_show.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __APP_SHOW__
#define __APP_SHOW__

#include "Inf_led.h"
#include "App_dataTime.h"
#include "App_freeRTOS.h"
#include "FreeRTOS.h"
#include "task.h"

typedef enum
{
    Show_Mode_Normal,
    Show_Mode_Time_Set,
    Show_Mode_Alarm_Set,
    Show_Mode_Volume_Set,
}SHOW_MODE_TYPE;



void App_show_Init(void);


//一般模式的LED显示
void App_show_Normal(Clock_Data_Time_Type *Clock_Data_Time,int8_t tempreture,int8_t humidity,Show_Type_Struct *Show_Type);

//时间设置模式下的led显示
void App_show_time_set(Clock_Data_Time_Type *Clock_Data_Time,int8_t tempreture,int8_t humidity,Show_Type_Struct *Show_Type);

/*
 * @Function: App_show_alarm_set
 * @Description: 闹钟设置模式下的LED显示
 */
void App_show_alarm_set(Clock_Data_Time_Type *Clock_Data_Time,int8_t tempreture,int8_t humidity,Show_Type_Struct *Show_Type);

#endif
