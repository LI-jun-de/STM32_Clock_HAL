/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2026-03-19 10:03:50
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2026-03-21 15:07:19
 * @FilePath: \MDK-ARM\application\App_switch.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "App_switch.h"

extern Clock_Data_Time_Type clock_data_time;

//展示类型，可以在正常显示时通过按键修改
extern Show_Type_Struct Show_Type;

extern Show_alarm_set show_alarm_set;

uint8_t alarm_data= 0;
extern uint8_t LED_EN;

//设置的两个闹钟
extern Alarm_time_type alarm_time_1;
extern Alarm_time_type alarm_time_2;

//展示模式
extern SHOW_MODE_TYPE Show_Mode;
extern Show_Time_Set show_time_set;

uint8_t sound_flag = 0;
uint32_t sound_control_last_time = 0;

//处理正常显示时开关的按键事件
void App_switch_key_process(void)
{
    //先获取按键开关的值
    key_type_value key_value = Inf_get_key_value();

    switch (key_value)//12/24小时切换
    {
    case KEY_TIME_SET:
        Show_Type.is_12_hour =!Show_Type.is_12_hour;
        break;
    
    case KEY_TIME_SET_LONG:
        //改变了显示的状态，由普通状态转变为时间设置状态
        Show_Mode = Show_Mode_Time_Set;
        //时间设置统一使用24小时制度
        Show_Type.is_12_hour = 0;
        if(clock_data_time.is_12_hour == 1 && clock_data_time.is_pm == 1)
        {
            clock_data_time.hour += 12;
            clock_data_time.is_12_hour = 0;
        }
        break;
    
    case KEY_UP://切换摄氏度和华氏度
        Show_Type.is_celsius =! Show_Type.is_celsius;
        break;

    case KEY_DOWN:
        //TODO:播放音频和调节音量
        Show_Mode = Show_Mode_Volume_Set;
        break;

    case KEY_ALARM_SET:
        //TODO:播放音频和调节音量
        Show_Mode = Show_Mode_Volume_Set;
        break;

    case KEY_ALARM_SET_LONG:
        //TODO：闹钟设置
        Show_Mode = Show_Mode_Alarm_Set;
        //进入闹钟设置模式后，统一使用24小时制
        Show_Type.is_12_hour = 0;
        Show_Type.is_alarm_1 = 1;
        Show_Type.is_alarm_2 = 0;
        break;
    
    case KEY_ALARM_EN:
        alarm_data++;
        if(alarm_data % 2 ==0)
        {
            Show_Type.is_alarm_1 = 0;
            Show_Type.is_alarm_2 = 0;
        }
        else if(alarm_data ==1)
        {
            Show_Type.is_alarm_1 = 1;
        }
        else if(alarm_data == 3)
        {
            Show_Type.is_alarm_2 = 1;
        }
        else if(alarm_data == 5)
        {
            Show_Type.is_alarm_1 = 1;
            Show_Type.is_alarm_2 = 1;            
        }
        else if(alarm_data > 5)
        {
            alarm_data = 0;
        }
        break;
    
    case KEY_ALARM_5:
        Show_Type.is_alarm_five = !Show_Type.is_alarm_five;
        break;
    default:
        break;
    }


}

//拨动开关处理
void App_switch_rotary_process(void)
{
    LED_ON_Type_value led_en_value = Inf_get_led_on_value();
    LIGHT_Type_value light_en_value = Inf_get_light_value();

    mic_value mic_en_value = Inf_get_mic_value();
    if(mic_en_value == MIC_ON)
    {
        sound_flag = 1;
        sound_control_last_time = xTaskGetTickCount();
    }

    //先检测总开关
    if(led_en_value == LED_ON)
    {
        //再检测是声控还是常亮模式
        if(light_en_value == LIGHT_ON)//如果是常亮模式
        {
            LED_EN = 1;
        }
        else if(light_en_value == LIGHT_OFF)//如果是声控模式
        {
            if(sound_flag)//如果有人说话
            {
                if(xTaskGetTickCount() - sound_control_last_time <= 5000)
                {
                    LED_EN = 1;
                }
                else
                {
                    LED_EN = 0;
                    sound_flag = 0;
                }
            }
            else//没人说话
            {
                LED_EN = 0;
            }
        }
        
    }
    else
    {
        LED_EN = 0;
    }
}

//时间设置状态时的按键处理
void App_switch_time_set_process(void)
{
    key_type_value key_value = Inf_get_key_value();

    switch (key_value)
    {
    case KEY_UP:
        //当前值+1
        if(show_time_set == Time_Set_Year)
        {
            clock_data_time.year ++;
            if(clock_data_time.year > 2099)
            {
                clock_data_time.year = 2000;
            }
        }
        else if(show_time_set == Time_Set_Month)
        {
            clock_data_time.month ++;
            if(clock_data_time.month > 12)
            {
                clock_data_time.month = 1;
            }
        }
        else if(show_time_set == Time_Set_Day)
        {
            clock_data_time.day ++;
            if(clock_data_time.day > 31)
            {
                clock_data_time.day = 1;
            }
        }
        else if (show_time_set == Time_Set_Hour)
        {
            clock_data_time.hour ++;
            if(clock_data_time.hour > 23)
            {
                clock_data_time.hour = 0;
            }
        }
        else if(show_time_set == Time_Set_Min)
        {
            clock_data_time.minute ++;
            if(clock_data_time.minute > 59)
            {
                clock_data_time.minute = 0;
            }
        }
        
        break;
    
    case KEY_DOWN:
        //当前值-1
        if(show_time_set == Time_Set_Year)
        {
            clock_data_time.year --;
            if(clock_data_time.year < 2000)
            {
                clock_data_time.year = 2099;
            }
        }
        else if(show_time_set == Time_Set_Month)
        {
            clock_data_time.month --;
            if(clock_data_time.month < 1)
            {
                clock_data_time.month = 12;
            }
        }
        else if(show_time_set == Time_Set_Day)
        {
            clock_data_time.day --;
            if(clock_data_time.day < 1 )
            {
                clock_data_time.day = 31;
            }
        }
        else if (show_time_set == Time_Set_Hour)
        {
            clock_data_time.hour --;
            if(clock_data_time.hour < 0)
            {
                clock_data_time.hour = 23;
            }
        }
        else if(show_time_set == Time_Set_Min)
        {
            clock_data_time.minute --;
            if(clock_data_time.minute < 0)
            {
                clock_data_time.minute = 59;
            }
        }
        
        break;
    
    case KEY_TIME_SET:
        //切换到下一个页面 如果已经是最后一个页面，则退出时间设置模式
        show_time_set ++;
        if(show_time_set == Time_Set_End)
        {
            //退出设置
            Show_Mode = Show_Mode_Normal;

            //设置时间
            clock_data_time.is_12_hour = Show_Type.is_12_hour;
            clock_data_time.is_pm = Show_Type.is_pm;
            App_dataTime_set(&clock_data_time);
            show_time_set = Time_Set_Year;
        }
        
        break;
    default:
        break;
    }
}

//闹钟设置状态时的按键处理
void App_switch_alarm_set_process(void)
{
    key_type_value key_value = Inf_get_key_value();
    //有四个按键
    switch (key_value)
    {
    case KEY_UP:
        //上调
        if(show_alarm_set == Alarm_Set_Hour)
        {
            if(Show_Type.is_alarm_1 == 1)
            {
                alarm_time_1.hour ++;
                if(alarm_time_1.hour > 23)
                {
                    alarm_time_1.hour = 0;
                }
            }
            else if(Show_Type.is_alarm_2 == 1)
            {
                alarm_time_2.hour ++;
                if(alarm_time_2.hour > 23)
                {
                    alarm_time_2.hour = 0;
                }
            }
        }
        else if(show_alarm_set == Alarm_Set_Minute)
        {
            if(Show_Type.is_alarm_1 == 1)
            {
                alarm_time_1.minute ++;
                if(alarm_time_1.minute > 59)
                {
                    alarm_time_1.minute = 0;
                }
            }
            else if(Show_Type.is_alarm_2 == 1)
            {
                alarm_time_2.minute ++;
                if(alarm_time_2.minute > 59)
                {
                    alarm_time_2.minute = 0;
                }
            }
        }
        break;
    case KEY_DOWN:
        //下调
        if(show_alarm_set == Alarm_Set_Hour)
        {
            if(Show_Type.is_alarm_1 == 1)
            {
                alarm_time_1.hour --;
                if(alarm_time_1.hour < 0)
                {
                    alarm_time_1.hour = 23;
                }
                
            }
            else if(Show_Type.is_alarm_2 == 1)
            {
                alarm_time_2.hour --;
                if(alarm_time_2.hour < 0)
                {
                    alarm_time_2.hour = 23;
                }
            }
        }
        else if(show_alarm_set == Alarm_Set_Minute)
        {
            if(Show_Type.is_alarm_1 == 1)
            {
                alarm_time_1.minute --;
                if(alarm_time_1.minute < 0)
                {
                    alarm_time_1.minute = 59;
                }
            }
            else if(Show_Type.is_alarm_2 == 1)
            {
                alarm_time_2.minute --;
                if(alarm_time_2.minute < 0)
                {
                    alarm_time_2.minute = 59;
                }
            }
        }
        break;
    case KEY_ALARM_SET:
        show_alarm_set ++;
        if(show_alarm_set == Alarm_Set_End)
        {   
            //切换到下一个页面 如果已经是最后一个页面，则退出闹钟设置模式 同时保存数据到全局变量
            //设置完闹钟之后，要让switch_alarm_en的值对应当前的设置
            if(alarm_data % 2 == 0)
            {
                //之前没有打开闹钟，现在设置哪个闹钟就打开哪个闹钟
                if(Show_Type.is_alarm_1 == 1)
                {
                    alarm_data = 1;
                }
                else if(Show_Type.is_alarm_2 == 1)
                {
                    alarm_data = 3;
                }
            }
            else if(alarm_data == 1)
            {
                //之前打开了闹钟1，现在设置了闹钟2，两个闹钟都打开
                if(Show_Type.is_alarm_2 == 1)
                {
                    alarm_data = 5;
                    Show_Type.is_alarm_1 = 1;
                }
            }
            else if(alarm_data == 3)
            {
                //之前打开了闹钟2，现在设置了闹钟1，两个闹钟都打开
                if(Show_Type.is_alarm_1 == 1)
                {
                    alarm_data = 5;
                    Show_Type.is_alarm_2 = 1;
                }
            }
            Show_Mode = Show_Mode_Normal;
            show_alarm_set = Alarm_Set_Hour;
           
        }
        

        break;
    case KEY_ALARM_EN:
        //闹钟12切换
        if(Show_Type.is_alarm_1 == 1)
        {
            Show_Type.is_alarm_1 = 0;
            Show_Type.is_alarm_2 = 1;
        }
        else if(Show_Type.is_alarm_2 == 1)
        {
            Show_Type.is_alarm_1 = 1;
            Show_Type.is_alarm_2 = 0;
        }
        break;
    default:
        break;
    }
}
