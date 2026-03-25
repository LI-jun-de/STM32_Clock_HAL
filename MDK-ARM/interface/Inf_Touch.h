#ifndef __INF_TOUCH__
#define __INF_TOUCH__

#include "GPIO.h"
#include "freertos.h"
#include "task.h"

#endif

typedef enum
{
    TOUCH_NONE = 0,
    TOUCH_UP,
} touch_type_value;

touch_type_value Inf_get_touch_value(void);


