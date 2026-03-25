/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2026-03-16 09:10:56
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2026-03-16 19:23:45
 * @FilePath: \MDK-ARM\interface\Inf_DHT11.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __INF_DHT11__
#define __INF_DHT11__


#include "gpio.h"
#include "Inf_DS1302Z.h"
#include "Com_debug.h"
#include "freertos.h"
#include "task.h"

void Inf_DHT11_Init(void);

#define DHT11_DATA_H HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin, GPIO_PIN_SET)
#define DHT11_DATA_L HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin, GPIO_PIN_RESET)
#define DHT11_DATA_READ HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin)


/**
 * @brief DHT11温湿度传感器初始化函数,需要延迟一秒
 * 
 */
void Inf_DHT11_Init(void);

/**
 * @brief 获取DHT11温湿度传感器的温湿度 只保留整数部分
 * 
 */
void Inf_DHT11_Get_Temp_Humi(int8_t *temp, int8_t *humi);



#endif
