/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2026-03-11 17:02:56
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2026-03-21 13:29:26
 * @FilePath: \MDK-ARM\application\App_freeRTOS.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "App_freeRTOS.h"

// 采集任务 毫秒级延迟 优先级低
void task_collect(void *pvParameters);
#define COLLECT_TASK_STACK_SIZE 128
#define COLLECT_TASK_PRIORITY 1
TaskHandle_t collect_task_handle;
// 全局变量保存温湿度的值
int8_t tempreture = 0;
int8_t humidity = 0;

// 全局变量保存时间信息
Clock_Data_Time_Type clock_data_time = {0};

// 展示任务
void task_show(void *pvParameters);
#define SHOW_TASK_STACK_SIZE 128
#define SHOW_TASK_PRIORITY 2
TaskHandle_t show_task_handle;
// 当前展示的状态
SHOW_MODE_TYPE Show_Mode = Show_Mode_Normal;

Show_Type_Struct Show_Type = {.is_12_hour = 0, .is_alarm_1 = 0, .is_alarm_2 = 0, .is_alarm_five = 0, .is_celsius = 1, .is_charging = 1, .is_pm = 0};

// 时间设置模式的不同状态,设置年、月、日...
Show_Time_Set show_time_set = Time_Set_Year;

// 闹钟设置模式的不同状态，设置小时、分钟
Show_alarm_set show_alarm_set = Alarm_Set_Hour;

// 开关检测任务
void task_switch(void *pvParameters);
#define SWITCH_TASK_STACK_SIZE 128
#define SWITCH_TASK_PRIORITY 3
TaskHandle_t switch_task_handle;

// 触摸开关的计数值
uint8_t touch_count = 0;

// 灯板的总开关
uint8_t LED_EN = 1;

// 闹钟结构体
Alarm_time_type alarm_time_1 = {0};
Alarm_time_type alarm_time_2 = {0};

// 闹钟任务
void task_alarm(void *pvParameters);
#define ALARM_TASK_STACK_SIZE 128
#define ALARM_TASK_PRIORITY 4
TaskHandle_t alarm_task_handle;

int8_t nv020d_volume = 5;
//调节音量任务
void task_volume(void *pvParameters);
#define VOLUME_TASK_STACK_SIZE 128
#define VOLUME_TASK_PRIORITY 4
TaskHandle_t volume_task_handle;

/**
 * @brief FreeRTOS应用层初始化函数,在main函数中调用，后面的代码不再生效
 *
 */
void App_freeRTOS_Init(void)
{
    // 1.创建任务
    // （1）task function 任务函数
    // （2）name 任务名字，无实质作用
    // （3）taskstacksize 任务栈大小，单位是四字节
    // （4）参数 传递给任务函数的参数
    // （5）优先级 0-4，数字越大优先级越高 范围是0-4，数字越大优先级越高
    // （6）任务句柄 任务的唯一标识符，创建任务成功之后会返回一个句柄，后续可以通过这个句柄来操作这个任务
    xTaskCreate(task_collect, "collect", COLLECT_TASK_STACK_SIZE, NULL, COLLECT_TASK_PRIORITY, &collect_task_handle);

    xTaskCreate(task_show, "show", SHOW_TASK_STACK_SIZE, NULL, SHOW_TASK_PRIORITY, &show_task_handle);

    xTaskCreate(task_switch, "switch", SWITCH_TASK_STACK_SIZE, NULL, SWITCH_TASK_PRIORITY, &switch_task_handle);

    xTaskCreate(task_alarm, "alarm", ALARM_TASK_STACK_SIZE, NULL, ALARM_TASK_PRIORITY, &alarm_task_handle);

    xTaskCreate(task_volume, "volume", VOLUME_TASK_STACK_SIZE, NULL, VOLUME_TASK_PRIORITY, &volume_task_handle);
    // 2.启动调度器
    vTaskStartScheduler();
}

void task_collect(void *pvParameters)
{
    // 1.初始化DHT11
    Inf_DHT11_Init();
    uint8_t task_collect_count = 9;
    while (1)
    {
        task_collect_count++;
        if (task_collect_count >= 10)
        {
            task_collect_count = 0;
            // 采集温湿度值
            Inf_DHT11_Get_Temp_Humi(&tempreture, &humidity);
        }

        // 如果进入到时间设置，停止采集时间
        if (Show_Mode != Show_Mode_Time_Set)
        {
            // 采集实时时钟模块的时间
            App_dataTime_get(&clock_data_time);
        }

        vTaskDelay(1000);
    }
}

void task_show(void *pvParameters)
{
    App_show_Init();
    while (1)
    {
        // 先判断LED的总开关
        if (LED_EN)
        {
            // 再判断展示的状态
            if (Show_Mode == Show_Mode_Normal || Show_Mode == Show_Mode_Volume_Set)
            {
                App_show_Normal(&clock_data_time, humidity, tempreture, &Show_Type);
            }
            else if (Show_Mode == Show_Mode_Time_Set)
            {
                App_show_time_set(&clock_data_time, humidity, tempreture, &Show_Type);
            }
            else if (Show_Mode == Show_Mode_Alarm_Set)
            {
                App_show_alarm_set(&clock_data_time, humidity, tempreture, &Show_Type);
            }
        }
        else
        {
            Inf_LED_Stop();
        }
    }
}

void task_switch(void *pvParameters)
{
    while (1)
    {
        // 按键开关
        if (Show_Mode == Show_Mode_Normal)
        {
            App_switch_key_process();
            vTaskDelay(50);
        }
        else if (Show_Mode == Show_Mode_Time_Set)
        {
            App_switch_time_set_process();
        }
        else if (Show_Mode == Show_Mode_Alarm_Set)
        {
            App_switch_alarm_set_process();
        }
        // 拨动开关
        App_switch_rotary_process();

        // 触摸开关
        touch_type_value touch_value = Inf_get_touch_value();
        if (touch_value == TOUCH_UP)
        {
            touch_count++;
            if (touch_count >= 5)
            {
                touch_count = 0;
            }
        }

        vTaskDelay(50);
    }
}

void task_alarm(void *pvParameters)
{
    Inf_NV020D_Init();
    uint8_t count_alarm = 0;
    uint8_t alarm_touch_count = 0;
    while (1)
    {
        // 如果是五天闹钟，要判断今天是周几
        if (Show_Type.is_alarm_five && clock_data_time.week >= 6)
        {
            // 如果是周末，则不需要响
        }
        else
        {
            // 工作日正常判断
            // 只有正常展示状态下才会检测闹钟
            if (Show_Mode == Show_Mode_Normal)
            {
                // 判断闹钟1
                if (Show_Type.is_alarm_1)
                {
                    if (clock_data_time.hour == alarm_time_1.hour && clock_data_time.minute == alarm_time_1.minute)
                    {
                        alarm_touch_count = touch_count;
                        if (alarm_time_1.is_on == 0)
                        {
                            // 让闹钟响
                            Inf_NV020D_Send_Command(0x00);
                            alarm_time_1.is_on = 1;
                        }
                        else if (alarm_time_1.is_on == 1 && alarm_time_1.is_down == 0) // 闹钟还没有响过
                        {
                            // 闹钟已经响了，等待一分钟关闭 或者触摸关闭
                            count_alarm++;
                            if (count_alarm >= 5)
                            {
                                Inf_NV020D_Send_Command(0x00);
                                count_alarm = 0;
                            }

                            if (alarm_touch_count != touch_count)
                            {
                                // 触摸了，关闭闹钟
                                Inf_NV020D_Send_Command(0xfe);
                                alarm_time_1.is_down = 1;
                            }
                        }
                    }
                }
                else
                {
                    // 响了超过一分钟，重置闹钟状态
                    if (alarm_time_1.is_on == 1)
                    {
                        alarm_time_1.is_on = 0;
                        alarm_time_1.is_down = 0;
                        Inf_NV020D_Send_Command(0xfe);
                    }
                }
                // 判断闹钟2
                if (Show_Type.is_alarm_2)
                {
                    if (clock_data_time.hour == alarm_time_2.hour && clock_data_time.minute == alarm_time_2.minute)
                    {
                        alarm_touch_count = touch_count;
                        if (alarm_time_2.is_on == 0)
                        {
                            // 让闹钟响
                            Inf_NV020D_Send_Command(0x00);
                            alarm_time_2.is_on = 1;
                        }
                        else if (alarm_time_2.is_on == 1 && alarm_time_2.is_down == 0) // 闹钟还没有响过
                        {
                            // 闹钟已经响了，等待一分钟关闭 或者触摸关闭
                            count_alarm++;
                            if (count_alarm >= 5)
                            {
                                Inf_NV020D_Send_Command(0x00);
                                count_alarm = 0;
                            }

                            if (alarm_touch_count != touch_count)
                            {
                                // 触摸了，关闭闹钟
                                Inf_NV020D_Send_Command(0xfe);
                                alarm_time_2.is_down = 1;
                            }
                        }
                    }
                }
                else
                {
                    // 响了超过一分钟，重置闹钟状态
                    if (alarm_time_2.is_on == 1)
                    {
                        alarm_time_2.is_on = 0;
                        alarm_time_2.is_down = 0;
                        Inf_NV020D_Send_Command(0xfe);
                    }
                }
            }
        }
        vTaskDelay(1000);
    }
}

void task_volume(void *pvParameters)
{
    //初始化音量
    Inf_NV020D_Init();
    Inf_NV020D_Set_Volume(nv020d_volume);
    uint8_t volume_touch_count = 99;
    while (1)
    {
        if (Show_Mode == Show_Mode_Volume_Set)
        {
            volume_touch_count ++;
            if(volume_touch_count >= 100)
            {
                //循环播放
                Inf_NV020D_Send_Command(0x00);
                volume_touch_count = 0;
            }

            //接收按键
            uint8_t key_value = Inf_get_key_value();
            switch (key_value)
            {
            case KEY_UP:
                //音量加
                nv020d_volume++;
                if(nv020d_volume > 7)
                {
                    nv020d_volume = 7;
                }
                Inf_NV020D_Set_Volume(nv020d_volume);
                break;
            
            case KEY_DOWN:
                //音量减
                nv020d_volume--;
                if(nv020d_volume < 0)
                {
                    nv020d_volume = 0;
                }
                Inf_NV020D_Set_Volume(nv020d_volume);
                break;
            
            case KEY_ALARM_SET:
                //退出音量设置
                Show_Mode = Show_Mode_Normal;
                Inf_NV020D_Send_Command(0xfe);
                volume_touch_count = 99;
                break;
            default:
                break;
            }
        }
        vTaskDelay(50);
    }
}
