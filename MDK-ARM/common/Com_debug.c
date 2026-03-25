#include "Com_debug.h"

//重映射函数
int fputc(int ch, FILE *f)
{
    //发送一个字节数据到串口
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);    
    return ch;
}
