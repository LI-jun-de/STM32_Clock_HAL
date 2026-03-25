#ifndef __APP_SWITCH__
#define __APP_SWITCH__

#include "Inf_key.h"
#include "Inf_led.h"
#include "Inf_mic.h"
#include "App_show.h"

//处理开关的按键事件
void App_switch_key_process(void);


void App_switch_rotary_process(void);

void App_switch_time_set_process(void);

void App_switch_alarm_set_process(void);

#endif
