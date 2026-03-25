/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2026-03-16 16:55:00
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2026-03-16 17:35:47
 * @FilePath: \MDK-ARM\interface\Inf_NV020D.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __INF_NV020D__
#define __INF_NV020D__

#include "gpio.h"
#include "Inf_DS1302Z.h"
#include "freertos.h"
#include "task.h"


//双线程控制
#define NV020D_SCK_H HAL_GPIO_WritePin(NVD_SCK_GPIO_Port, NVD_SCK_Pin, GPIO_PIN_SET)
#define NV020D_SCK_L HAL_GPIO_WritePin(NVD_SCK_GPIO_Port, NVD_SCK_Pin, GPIO_PIN_RESET)
#define NV020D_SDA_H HAL_GPIO_WritePin(NVD_SDA_GPIO_Port, NVD_SDA_Pin, GPIO_PIN_SET)
#define NV020D_SDA_L HAL_GPIO_WritePin(NVD_SDA_GPIO_Port, NVD_SDA_Pin, GPIO_PIN_RESET)
#define NV020D_BUZY_READ HAL_GPIO_ReadPin(NVD_BUZY_GPIO_Port, NVD_BUZY_Pin)


//单线程控制
#define NV020D_CTRL_H HAL_GPIO_WritePin(NVD_SCK_GPIO_Port, NVD_SCK_Pin, GPIO_PIN_SET)
#define NV020D_CTRL_L HAL_GPIO_WritePin(NVD_SCK_GPIO_Port, NVD_SCK_Pin, GPIO_PIN_RESET)

/**
 * @brief NV020D初始化
 */
void Inf_NV020D_Init(void);

/**
 * @brief NV020D发送字节
 * 
 * @param Byte 字节数据
 */
void Inf_NV020D_Send_Byte(uint8_t Byte);

/**
 * @brief NV020D发送命令
 * 
 * @param cmd 命令字
 */
void Inf_NV020D_Send_Command(uint8_t cmd);

/**
 * @brief NV020D设置音量
 * 
 * @param volume 音量值，范围0-7
 */
void Inf_NV020D_Set_Volume(uint8_t volume);

/**
 * @brief NV020D停止播放
 */
void Inf_NV020D_Stop(void);

#endif

