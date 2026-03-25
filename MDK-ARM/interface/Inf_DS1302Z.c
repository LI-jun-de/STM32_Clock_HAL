#include "Inf_DS1302Z.h"

// 延迟函数，单位是微秒,误差在0.006s左右
void Inf_Delay_Us(uint32_t us)
{
    uint32_t count = us * 8;
    while (count--)
    {
        __NOP();
    }
}

uint8_t Inf_DS1302Z_ReadReg(uint8_t reg)
{
    //确保是读指令
    reg |= 0x01;

    uint8_t Data = 0;
    // 1.初始化引脚
    DS_RST_L;
    DS_CLK_L;

    // 2.复位引脚拉高，进入通信状态
    DS_RST_H;
    Inf_Delay_Us(5);

    // 3.发送寄存器地址 循环八次
    for (int8_t i = 0; i < 8; i++)
    {
        if (reg & (1 << i))
        {
            DS_IO_H;
        }
        else
        {
            DS_IO_L;
        }
        DS_CLK_H;
        Inf_Delay_Us(1);

        DS_CLK_L;
    }

    // 发送完寄存器地址之后，切换引脚为输入模式
    DS_IO_H;

    // 4.读取寄存器的值 循环八次
    for (int8_t i = 0; i < 8; i++)
    {

        Data |= (DS_IO_READ << i);
        if (i < 7)
        {
            DS_CLK_H;
            DS_CLK_L;
        }
    }

    // 5.通信结束，复位引脚拉低
    DS_RST_L;

    return Data;
}

void Inf_DS1302Z_WriteReg(uint8_t reg, uint8_t value)
{
    //确保是写指令
    reg &= 0xFE;

    // 1.初始化引脚
    DS_RST_L;
    DS_CLK_L;

    // 2.复位引脚拉高，进入通信状态
    DS_RST_H;
    Inf_Delay_Us(5);

    // 3.发送寄存器地址 循环八次
    for (int8_t i = 0; i < 8; i++)
    {
        if (reg & (1 << i))
        {
            DS_IO_H;
        }
        else
        {
            DS_IO_L;
        }
        DS_CLK_H;
        Inf_Delay_Us(1);

        DS_CLK_L;
    }
    
    // 4.发送寄存器的值 循环八次
    for (int8_t i = 0; i < 8; i++)
    {
        if (value & (1 << i))
        {
            DS_IO_H;
        }
        else
        {
            DS_IO_L;
        }
        DS_CLK_H;
        Inf_Delay_Us(1);

        DS_CLK_L;
    }

    // 5.通信结束，复位引脚拉低
    DS_RST_L;
}
