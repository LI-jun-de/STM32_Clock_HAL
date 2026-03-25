#ifndef __APP_DATATIME__
#define __APP_DATATIME__

#include "Inf_DS1302Z.h"

typedef struct 
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    int8_t hour; //要能表示负数
    int8_t minute;
    uint8_t second;
    uint8_t week;
    uint8_t is_pm;
    uint8_t is_12_hour;
}Clock_Data_Time_Type;


typedef struct 
{
    int8_t hour;
    int8_t minute;
    uint8_t is_on;
    uint8_t is_down;
}Alarm_time_type;


void App_dataTime_get(Clock_Data_Time_Type *data_time);


//设置时间函数
void App_dataTime_set(Clock_Data_Time_Type *data_time);

#endif
