#ifndef __drv_timer_h
#define __drv_timer_h

#include "stm32f4xx_hal_def.h"

#define tim1_config


void time_config(void );
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);
void start_timer(void);
void stop_timer(void);
void restart_timer(void);


#endif

