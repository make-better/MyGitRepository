#ifndef _BSP_WWDG_H_
#define _BSP_WWDG_H_
#include "stm32f10x.h"

#define WWDG_CNT      0x7f



void WWDG_Config(uint8_t wr,uint8_t tr,uint32_t prv);
void WWDG_Feed(void);
#endif //_BSP_WWDG_H_
