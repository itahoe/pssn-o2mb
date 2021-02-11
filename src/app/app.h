#ifndef APP_H
#define APP_H



#include "stm32f0xx_hal.h"
#include <stdbool.h>
#include "app_error.h"
#include "app_trace.h"


/*******************************************************************************
*
*******************************************************************************/
#define SENS_CAL_NUM_POINTS             2

#define	UI_LED_FLSH_SHRT_TCKS   ( ((size_t) CFG_UI_LED_FLSH_SHRT_mSEC * STM32_SYSTICK_HZ) / 1000 )
#define	UI_LED_FLSH_LONG_TCKS   ( ((size_t) CFG_UI_LED_FLSH_LONG_mSEC * STM32_SYSTICK_HZ) / 1000 )


typedef union
{
        int32_t                 i32;
        uint32_t                u32;
        uint16_t                u16[ 2];
} app_union_t;

/*******************************************************************************
*
*******************************************************************************/
void
app_1msec_tick_hook( void );


void
app_ser1_recv_hook( void );

void
app_sens_hook( void );

void
app_adc_eos_hook( void );


#endif //APP_H

