/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2026-03-16 16:54:40
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2026-03-21 13:31:06
 * @FilePath: \MDK-ARM\interface\Inf_NV020D.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "Inf_NV020D.h"



/**
 * @brief NV020D初始化
 */
void Inf_NV020D_Init(void)
{
    vTaskDelay(500);
}

/**
 * @brief NV020D发送字节
 *
 * @param Byte 字节数据
 */
void Inf_NV020D_Send_Byte(uint8_t Byte)
{
    // 初始化SCL和SDA为高电平
    NV020D_SCK_H;

    // 提前将数据准备好，保证时钟拉低之后数据线上的数据是稳定的
    if (Byte & 1)
    {
        NV020D_SDA_H;
    }
    else
    {
        NV020D_SDA_L;
    }

    NV020D_SCK_L;
    vTaskDelay(4);
    for (int i = 0; i < 8; i++)
    {
        if (Byte & (1 << i))
        {
            NV020D_SDA_H;
        }
        else
        {
            NV020D_SDA_L;
        }
        Inf_Delay_Us(800);
        NV020D_SCK_L;
        Inf_Delay_Us(800);
        NV020D_SCK_H;
    }
    NV020D_SDA_H;
}

/**
 * @brief NV020D发送命令
 *
 * @param cmd 命令字
 */

 //双线程控制
// void Inf_NV020D_Send_Command(uint8_t cmd)
// {
//     // 等待buzy为低电平
//     while (NV020D_BUZY_READ == GPIO_PIN_RESET)
//     {
//         vTaskDelay(10);
//     }
//     vTaskDelay(100);
//     Inf_NV020D_Send_Byte(0xf1);
//     Inf_NV020D_Send_Byte(cmd);
//     Inf_NV020D_Send_Byte(0xf3);
//     uint8_t checksum = (uint8_t)(0xf1 + cmd + 0xf3);
//     Inf_NV020D_Send_Byte(checksum);
// }


//单线程控制
void Inf_NV020D_Send_Command(uint8_t cmd)
{
    NV020D_CTRL_H;
    
    NV020D_CTRL_L;
    vTaskDelay(4);

    for(uint8_t i = 0; i < 8; i++)
    {
        if (cmd & (1 << i))
        {
            NV020D_CTRL_H; //高低电平3：1
            Inf_Delay_Us(1200);
            // vTaskDelay(3); //为了不阻塞CPU 但是优先级必须是最高
            NV020D_CTRL_L;
            Inf_Delay_Us(400);
            // vTaskDelay(1);
        }
        else
        {
            NV020D_CTRL_H;
            Inf_Delay_Us(400); //高低电平1：3
            //  vTaskDelay(1); 
            NV020D_CTRL_L;
            Inf_Delay_Us(1200);
            //  vTaskDelay(3); 
        }
        
    }
    NV020D_CTRL_H;
    vTaskDelay(100);
}

/**
 * @brief NV020D设置音量
 * 
 * @param volume 音量值，范围0-7
 */
void Inf_NV020D_Set_Volume(uint8_t volume)
{
    if (volume > 7)
    {
        volume = 7; // 限制音量范围在0-7
    }
    Inf_NV020D_Send_Command(volume + 0xE0);
}

/**
 * @brief NV020D停止播放
 */
void Inf_NV020D_Stop(void)
{
    Inf_NV020D_Send_Command(0xFE);
}
