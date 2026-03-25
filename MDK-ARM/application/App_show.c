/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2026-03-17 20:35:36
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2026-03-20 18:29:50
 * @FilePath: \MDK-ARM\application\App_show.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "App_show.h"

uint8_t clock_flag = 0;
uint32_t clock_last_time;

extern Alarm_time_type alarm_time_1;
extern Alarm_time_type alarm_time_2;

//时间设置页面
extern Show_Time_Set show_time_set;

//闹钟设置页面
extern Show_alarm_set show_alarm_set;

void App_show_Init(void)
{
    Inf_LED_Start();
    clock_last_time = xTaskGetTickCount();
}


//一般模式的LED显示
void App_show_Normal(Clock_Data_Time_Type *Clock_Data_Time,int8_t humidity,int8_t tempreture,Show_Type_Struct *Show_Type)
{
    uint8_t hour;
    // Inf_LED_Set_Clock(Clock_Data_Time->hour,Clock_Data_Time->minute,clock_flag);
    if(xTaskGetTickCount() - clock_last_time >= 500)
    {
        clock_flag =! clock_flag;

        clock_last_time = xTaskGetTickCount();
    }
    //统一转换为24小时制，因为在设置时间的时候都是按照24小时制进行设置的
    if(Clock_Data_Time->is_12_hour && Clock_Data_Time->is_pm)
    {
        hour = 12 + Clock_Data_Time->hour;
    }
    else
    {
        hour = Clock_Data_Time->hour;
    }

    if(Show_Type->is_12_hour)
    {
        if(hour > 12)
        {
            hour -= 12 ;
            Show_Type->is_pm = 1;  
        }
        else
        {
            Show_Type->is_pm = 0;
        }
    }

    Inf_led_set_all(hour,Clock_Data_Time->minute,clock_flag,humidity,tempreture,Show_Type);
    
}

//时间设置模式下的led显示
void App_show_time_set(Clock_Data_Time_Type *Clock_Data_Time,int8_t tempreture,int8_t humidity,Show_Type_Struct *Show_Type)
{
    if(xTaskGetTickCount() - clock_last_time >= 500)
    {
        clock_flag =! clock_flag;

        clock_last_time = xTaskGetTickCount();
    }
    //判断设置页面
    switch (show_time_set)
    {
    case Time_Set_Year:
        if(clock_flag)
        {
            // Inf_led_time_set(show_time_set,Clock_Data_Time->year);
            Inf_led_set_all(Clock_Data_Time->year / 100,Clock_Data_Time->year % 100 , 0 ,tempreture ,humidity,Show_Type);
        }
        else
        {
            Inf_led_set_all(Clock_Data_Time->year / 100, 0xff, 0, tempreture , humidity , Show_Type);
        }
        break;

    case Time_Set_Month:
        // Inf_led_time_set(show_time_set,Clock_Data_Time->month);
        if(clock_flag)
        {
            Inf_led_set_all(Clock_Data_Time->month, 0xff, 0, tempreture , humidity , Show_Type);
        }
        else
        {
            Inf_led_set_all(0xff, 0xff, 0, tempreture , humidity , Show_Type);
        }
        break;
    
    case Time_Set_Day:
        // Inf_led_time_set(show_time_set,Clock_Data_Time->day);
        if(clock_flag)
        {
            Inf_led_set_all(0xff, Clock_Data_Time->day, 0, tempreture , humidity , Show_Type);
        }
        else
        {
            Inf_led_set_all(0xff, 0xff, 0, tempreture , humidity , Show_Type);
        }
        break;

    case Time_Set_Hour:
        // Inf_led_time_set(show_time_set,Clock_Data_Time->hour);
        if(clock_flag)
        {
            Inf_led_set_all(Clock_Data_Time->hour, 0xff , 1, tempreture , humidity , Show_Type);
        }
        else
        {
            Inf_led_set_all(0xff, 0xff, 1, tempreture , humidity , Show_Type);
        }
        break;

    case Time_Set_Min:
        // Inf_led_time_set(show_time_set,Clock_Data_Time->minute);
        if(clock_flag)
        {
            Inf_led_set_all(0xff, Clock_Data_Time->minute , 1, tempreture , humidity , Show_Type);
        }
        else
        {
            Inf_led_set_all(0xff, 0xff, 1, tempreture , humidity , Show_Type);
        }
        break;

    default:
        break;
    }
}

/*
 * @Function: App_show_alarm_set
 * @Description: 闹钟设置模式下的LED显示
 */
void App_show_alarm_set(Clock_Data_Time_Type *Clock_Data_Time,int8_t tempreture,int8_t humidity,Show_Type_Struct *Show_Type)
{
    if(xTaskGetTickCount() - clock_last_time >= 500)
    {
        clock_flag =! clock_flag;

        clock_last_time = xTaskGetTickCount();
    }
    switch (show_alarm_set)
    {
    case Alarm_Set_Hour:
        if(clock_flag)
        {
            if(Show_Type->is_alarm_1)
            {
                Inf_led_set_all(alarm_time_1.hour, 0xff , 1, tempreture , humidity , Show_Type);
            }
            else if(Show_Type->is_alarm_2)
            {
                Inf_led_set_all(alarm_time_2.hour, 0xff , 1, tempreture , humidity , Show_Type);
            }
        }
        else
        {
            Inf_led_set_all(0xff, 0xff, 1, tempreture , humidity , Show_Type);
        }
        break;
    case Alarm_Set_Minute:
        if(clock_flag)
        {
            if(Show_Type->is_alarm_1)
            {
                Inf_led_set_all(0xff, alarm_time_1.minute , 1, tempreture , humidity , Show_Type);
            }
            else if(Show_Type->is_alarm_2)
            {
                Inf_led_set_all(0xff, alarm_time_2.minute , 1, tempreture , humidity , Show_Type);
            }
        }
        else
        {
            Inf_led_set_all(0xff, 0xff, 1, tempreture , humidity , Show_Type);
        }
        break;
    default:
        break;
    }
}
