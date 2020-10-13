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


typedef union
{
        int32_t                 i32;
        uint32_t                u32;
        uint16_t                u16[ 2];
} app_union_t;

typedef struct  app_evt_s
{
        bool                    ser1_recv;
        //bool                    lm75;
        bool                    tick_1hz;
        bool                    adc;
        bool                    sens;
} app_evt_t;


typedef struct  app_s
{
        app_evt_t               evt;
        size_t                  tick_1sec;
} app_t;


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

