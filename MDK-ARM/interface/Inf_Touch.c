#include "Inf_Touch.h"

touch_type_value Inf_get_touch_value(void)
{
    if (HAL_GPIO_ReadPin(TOUCH_GPIO_Port, TOUCH_Pin) == GPIO_PIN_RESET)
    {
        //´¥ÃþÏû¶¶
        vTaskDelay(100);
        if (HAL_GPIO_ReadPin(TOUCH_GPIO_Port, TOUCH_Pin) == GPIO_PIN_RESET)
        {
            return TOUCH_UP;
        }
    }
    //ÊÍ·ÅÏû¶¶
    else
    {
        vTaskDelay(100);
        if (HAL_GPIO_ReadPin(TOUCH_GPIO_Port, TOUCH_Pin) == GPIO_PIN_SET)
        {
            return TOUCH_NONE;
        }
    }
    return TOUCH_NONE;
}
