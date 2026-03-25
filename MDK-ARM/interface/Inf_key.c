#include "Inf_key.h"

uint8_t Inf_get_key_press(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    // 确认短按按键值的逻辑 所有按键默认值是1
    // 1.确认按键被按下
    if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
    {
        // 按键被按下了
        vTaskDelay(10); // 消抖
        if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
        {
            while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            return 1;
        }
    }

    return 0;
}

/**
 * @brief 获取按键值的函数
 * @return key_type_value 枚举类型，返回对应的按键值 NONE表示没有按键被按下，其他值表示对应的按键事件
 */
key_type_value Inf_get_key_value(void)
{
    // 确认短按按键值的逻辑 所有按键默认值是1
    // 1.确认按键被按下
    if(Inf_get_key_press(UP_GPIO_Port, UP_Pin) == 1)
    {
        return KEY_UP;
    }
    else if(Inf_get_key_press(DOWN_GPIO_Port, DOWN_Pin) == 1)
    {
        return KEY_DOWN;
    }
    else if(Inf_get_key_press(ALARM_5_GPIO_Port, ALARM_5_Pin) == 1)
    {
        return KEY_ALARM_5;
    }
    else if(Inf_get_key_press(ALARM_EN_GPIO_Port, ALARM_EN_Pin) == 1)
    {
        return KEY_ALARM_EN;
    }
    
    if(HAL_GPIO_ReadPin(TIME_SET_GPIO_Port, TIME_SET_Pin) == GPIO_PIN_RESET)
    {
        // 确认长按按键值的逻辑
        vTaskDelay(10); // 消抖
        if(HAL_GPIO_ReadPin(TIME_SET_GPIO_Port, TIME_SET_Pin) == GPIO_PIN_RESET)
        {
           TickType_t press_start_time = xTaskGetTickCount(); // 记录按键按下的时间
            while(HAL_GPIO_ReadPin(TIME_SET_GPIO_Port, TIME_SET_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            TickType_t press_duration = xTaskGetTickCount() - press_start_time; // 计算按键持续的时间
            if(press_duration >= pdMS_TO_TICKS(3000)) // 长按时间阈值
            {
                return KEY_TIME_SET_LONG;
            }
            else
            {
                return KEY_TIME_SET;
            }
        }
    }
    if(HAL_GPIO_ReadPin(ALARM_SET_GPIO_Port, ALARM_SET_Pin) == GPIO_PIN_RESET)
    {
        // 确认长按按键值的逻辑
        vTaskDelay(10); // 消抖
        if(HAL_GPIO_ReadPin(ALARM_SET_GPIO_Port, ALARM_SET_Pin) == GPIO_PIN_RESET)
        {
           TickType_t press_start_time = xTaskGetTickCount(); // 记录按键按下的时间
            while(HAL_GPIO_ReadPin(ALARM_SET_GPIO_Port, ALARM_SET_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            TickType_t press_duration = xTaskGetTickCount() - press_start_time; // 计算按键持续的时间
            if(press_duration >= pdMS_TO_TICKS(3000)) // 长按时间阈值
            {
                return KEY_ALARM_SET_LONG;
            }
            else
            {
                return KEY_ALARM_SET;
            }
        }

    }
    return KEY_NONE;
}


/**
 * @brief 获取LED状态的函数
 * @return LED_ON_Type_value 枚举类型，返回LED的状态 LED_OFF表示LED关闭，LED_ON表示LED打开
 */
LED_ON_Type_value Inf_get_led_on_value(void)
{
    if(HAL_GPIO_ReadPin(LED_ON_GPIO_Port, LED_ON_Pin) == GPIO_PIN_SET)
    {
        //对应开灯状态
        return LED_ON;
    } 
    return LED_OFF;
}
 

/**
 * @brief 获取当前是否为常亮的函数
 * @return LIGHT_Type_value
 */
LIGHT_Type_value Inf_get_light_value(void)
{
    if(HAL_GPIO_ReadPin(LIGHT_GPIO_Port, LIGHT_Pin) == GPIO_PIN_SET)
    {
        //对应常亮状态
        return LIGHT_ON;
    } 
    return LIGHT_OFF;
}
