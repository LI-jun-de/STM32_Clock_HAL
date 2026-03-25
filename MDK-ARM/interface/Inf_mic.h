#ifndef __INF_MIC__
#define __INF_MIC__

#include "gpio.h"

typedef enum
{
    MIC_ON,
    MIC_OFF,
} mic_value;

mic_value Inf_get_mic_value(void);


#endif
