/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2026-03-14 18:13:13
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2026-03-16 19:24:56
 * @FilePath: \MDK-ARM\interface\Inf_DS1302Z.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __INF_DS1302Z__
#define __INF_DS1302Z__

#include "gpio.h"

#define DS1302Z_SECOND_REG 0x80
#define DS1302Z_MINUTE_REG 0x82
#define DS1302Z_HOUR_REG 0x84
#define DS1302Z_DAY_REG 0x86
#define DS1302Z_MONTH_REG 0x88
#define DS1302Z_WEEK_REG 0x8A
#define DS1302Z_YEAR_REG 0x8C

//控制寄存器
#define DS1302Z_CONTROL_REG 0x8E

#define DE1302Z_WRITE_REG 0x00
#define DE1302Z_READ_REG 0x01

void Inf_Delay_Us(uint32_t us);

//读一个寄存器的值
uint8_t Inf_DS1302Z_ReadReg(uint8_t reg);
//写一个寄存器的值
void Inf_DS1302Z_WriteReg(uint8_t reg, uint8_t value);


//控制引脚
#define DS_CLK_H HAL_GPIO_WritePin(DS_CLK_GPIO_Port, DS_CLK_Pin, GPIO_PIN_SET)
#define DS_CLK_L HAL_GPIO_WritePin(DS_CLK_GPIO_Port, DS_CLK_Pin, GPIO_PIN_RESET)
#define DS_IO_H HAL_GPIO_WritePin(DS_IO_GPIO_Port, DS_IO_Pin, GPIO_PIN_SET)
#define DS_IO_L HAL_GPIO_WritePin(DS_IO_GPIO_Port, DS_IO_Pin, GPIO_PIN_RESET)

//读写引脚
#define DS_IO_READ HAL_GPIO_ReadPin(DS_IO_GPIO_Port, DS_IO_Pin)

//复位引脚
#define DS_RST_H HAL_GPIO_WritePin(DS_RST_GPIO_Port, DS_RST_Pin, GPIO_PIN_SET)
#define DS_RST_L HAL_GPIO_WritePin(DS_RST_GPIO_Port, DS_RST_Pin, GPIO_PIN_RESET)

#endif
