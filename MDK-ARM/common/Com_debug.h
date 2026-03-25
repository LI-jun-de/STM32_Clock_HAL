#ifndef __COM_DEBUG_H__
#define __COM_DEBUG_H__

#include "stdarg.h"
#include "stdio.h"
#include "usart.h"

//1.设置日志打印开关
//
#define DEBUG_ENABLE 1

#ifdef DEBUG_ENABLE
#define DEBUG_PRINT(format, ...) printf("[%s:%d] " format "\r\n", __FILE__, __LINE__, ##__VA_ARGS__)

#else
#define DEBUG_PRINT(...)

#endif

#endif
