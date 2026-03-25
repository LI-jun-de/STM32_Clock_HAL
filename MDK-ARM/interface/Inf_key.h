#ifndef __INF_KEY_H__
#define __INF_KEY_H__

#include "gpio.h"
#include "Com_debug.h"
#include "freertos.h"
#include "task.h"

typedef enum
{
    KEY_NONE = 0,
    KEY_TIME_SET,       // 表示第一个按键的短按
    KEY_TIME_SET_LONG,  // 表示第一个按键的长按
    KEY_UP,             // 上调
    KEY_DOWN,           // 下调
    KEY_ALARM_SET,      // 表示第二个按键的短按
    KEY_ALARM_SET_LONG, // 表示第二个按键的长按
    KEY_ALARM_EN,       // 闹钟使能
    KEY_ALARM_5,        // 五天闹钟
} key_type_value;

typedef enum
{
    LED_OFF = 0,
    LED_ON
} LED_ON_Type_value;

typedef enum
{
    LIGHT_OFF = 0,
    LIGHT_ON

} LIGHT_Type_value;


/**
* @brief 获取按键值的函数
* @return key_type_value 枚举类型，返回对应的按键值 NONE表示没有按键被按下，其他值表示对应的按键事件
*/
key_type_value Inf_get_key_value(void);

/**
 * @brief 获取LED状态的函数
 * @return LED_ON_Type_value 枚举类型，返回LED的状态 LED_OFF表示LED关闭，LED_ON表示LED打开
 */
LED_ON_Type_value Inf_get_led_on_value(void);

/**
 * @brief 获取当前是否为常亮的函数
 * @return LIGHT_Type_value 
 */
LIGHT_Type_value Inf_get_light_value(void);

#endif
