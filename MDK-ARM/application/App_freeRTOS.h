#ifndef __APP_FREERTOS_H__
#define __APP_FREERTOS_H__

#include "FreeRTOS.h"
#include "task.h"
#include "Com_debug.h"
#include "Inf_key.h"
#include "Inf_Touch.h"
#include "Inf_mic.h"
#include "Inf_DS1302Z.h"
#include "Inf_DHT11.h"
#include "Inf_NV020D.h"
#include "Inf_LED.h"

#include "App_dataTime.h"
#include "App_show.h"
#include "App_switch.h"
/**
 * @brief FreeRTOS应用层初始化函数,在main函数中调用，后面的代码不再生效
 * 
 */
void App_freeRTOS_Init(void);


#endif
