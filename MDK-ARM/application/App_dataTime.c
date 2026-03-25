/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2026-03-17 19:26:13
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2026-03-23 11:04:37
 * @FilePath: \MDK-ARM\application\App_dataTime.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "App_dataTime.h"

// 从时钟芯片采集时间
void App_dataTime_get(Clock_Data_Time_Type *data_time)
{
    // 1.读取年份
    uint8_t year_data = Inf_DS1302Z_ReadReg(DS1302Z_YEAR_REG);
    data_time->year = (year_data >> 4) * 10 + (year_data & 0x0f) + 2000;

    // 2.读取月份
    uint8_t month_data = Inf_DS1302Z_ReadReg(DS1302Z_MONTH_REG);
    data_time->month = (month_data >> 4) * 10 + (month_data & 0x0f);

    // 3.读取日
    uint8_t day_data = Inf_DS1302Z_ReadReg(DS1302Z_DAY_REG);
    data_time->day = (day_data >> 4) * 10 + (day_data & 0x0f);

    // 4.读取星期
    uint8_t week_data = Inf_DS1302Z_ReadReg(DS1302Z_WEEK_REG);
    data_time->week = (week_data & 0x0f);

    // 5.读取小时
    uint8_t hour_data = Inf_DS1302Z_ReadReg(DS1302Z_HOUR_REG);
    // 判断是12小时还是24小时制
    if (hour_data & 0x80)
    {
        // 十二小时
        data_time->is_12_hour = 1;
        // 判断是上午还是下午
        if (hour_data & 0x20)
        {
            // 下午
            data_time->is_pm = 1;
        }
        else
        {
            // 上午
            data_time->is_pm = 0;
        }
        if (hour_data & 0x10) //如果有超过10小时的情况
        {
            data_time->hour = 10 + (hour_data & 0x0f);
        }
        else
        {
            data_time->hour = (hour_data & 0x0f);
        }
    }
    else
    {
        // 二十四小时
        data_time->is_12_hour = 0;
        if (hour_data & 0x20) // 0010 0000 这一位表示有没有超过20
        {
            data_time->hour = 20 + (hour_data & 0x0f);
        }
        else if (hour_data & 0x10) // 表示有没有超过十
        {
            data_time->hour = 10 + (hour_data & 0x0f);
        }
        else
        {
            // 后四位表示时钟的个位
            data_time->hour = (hour_data & 0x0f);
        }
    }

    //统一使用24小时制保存
    if (data_time->is_12_hour && data_time->is_pm)
    {
        data_time->hour += 12;
        data_time->is_12_hour = 0;
    }

    // 6.读取分钟
    uint8_t minute_data = Inf_DS1302Z_ReadReg(DS1302Z_MINUTE_REG);
    data_time->minute = (minute_data >> 4) * 10 + (minute_data & 0x0f);
}

// 主控芯片将数据发送给时钟芯片，设置时间
void App_dataTime_set(Clock_Data_Time_Type *data_time)
{
    // 将控制寄存器的写保护置零
    Inf_DS1302Z_WriteReg(DS1302Z_CONTROL_REG, 0x00);

    // 写入年
    uint16_t year_shi;
    uint16_t year_ge;
    year_shi = (data_time->year % 100) / 10;
    year_ge = data_time->year % 10;
    Inf_DS1302Z_WriteReg(DS1302Z_YEAR_REG, (year_shi << 4) | year_ge);

    // 写入月
    uint16_t month_shi;
    uint16_t month_ge;
    month_shi = data_time->month / 10;
    month_ge = data_time->month % 10;
    Inf_DS1302Z_WriteReg(DS1302Z_MONTH_REG, (((month_shi & 0x01) << 4) | month_ge)); //&0x01只是为了保险起见

    // 写入日
    uint16_t day_shi = data_time->day / 10;
    uint16_t day_ge = data_time->day % 10;
    Inf_DS1302Z_WriteReg(DS1302Z_DAY_REG, (((day_shi & 0x03) << 4) | day_ge)); //&0x03只是为了保险起见，清零前两位用不到的数据

    // 写入小时 在时间设置data_time小时的时候都是采用24小时制进行设置，在这写入数据用12小时制
    if (data_time->hour > 12)
    {
        uint8_t hour_shi = data_time->hour / 10;
        uint8_t hour_ge = data_time->hour % 10;
        Inf_DS1302Z_WriteReg(DS1302Z_HOUR_REG, 0x80 | 0x20 | ((hour_shi) << 4) | hour_ge);
        // 0x80表示12小时制，0x20表示pm
    }
    else
    {
        uint8_t hour_shi = data_time->hour / 10;
        uint8_t hour_ge = data_time->hour % 10;
        Inf_DS1302Z_WriteReg(DS1302Z_HOUR_REG, 0x80 | ((hour_shi) << 4) | hour_ge);
    }

    // 写入分钟
    uint8_t min_shi = data_time->minute / 10;
    uint8_t min_ge = data_time->minute % 10;
    Inf_DS1302Z_WriteReg(DS1302Z_MINUTE_REG, ((min_shi) << 4) | min_ge);

    //写入秒，默认为零
    Inf_DS1302Z_WriteReg(DS1302Z_SECOND_REG,0x00);

    //计算星期，使用蔡勒公式
    uint16_t y = data_time->year;
    uint8_t c = y / 100;
    uint16_t m = data_time->month;
    if(m == 1 || m ==2)
    {
        y --;
        m += 12;
    }
    int16_t w = y + y / 4 + c / 4 - 2 * c + 26 *(m + 1) / 10 + data_time->day - 1;
    while (w < 0)
    {
        w += 7;
    }
    w = w % 7;
    if(w == 0)
    {
        w = 7;
    }
    Inf_DS1302Z_WriteReg(DS1302Z_WEEK_REG,w);
}
