#ifndef APP_H
#define APP_H


#include "stm32f0xx_hal.h"
#include <stdbool.h>
#include "app_error.h"
#include "app_trace.h"
#include "ad7799.h"


/*******************************************************************************
*
*******************************************************************************/
#define SENS_CAL_NUM_POINTS             2

#define	UI_LED_FLSH_SHRT_TCKS   ( ((size_t) CFG_UI_LED_FLSH_SHRT_mSEC * STM32_SYSTICK_HZ) / 1000 )
#define	UI_LED_FLSH_LONG_TCKS   ( ((size_t) CFG_UI_LED_FLSH_LONG_mSEC * STM32_SYSTICK_HZ) / 1000 )


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

/*******************************************************************************
* ADC
*******************************************************************************/
//void            app_afe_adc_reg_mode_set( const uint16_t );
//uint16_t        app_afe_adc_reg_mode_get( void );
//void            app_afe_adc_reg_conf_set( const uint16_t );
//uint16_t        app_afe_adc_reg_conf_get( void );
//int             app_afe_adc_psw_get( void );
//void            app_afe_adc_psw_set( const int );
//int             app_afe_adc_unipolar_get( void );
//void            app_afe_adc_unipolar_set( const int );
//int             app_afe_adc_buffer_enable_get( void );
//void            app_afe_adc_buffer_enable_set( const int );
//uint16_t        app_afe_adc_chnl_get( void );
//void            app_afe_adc_chnl_set( const uint16_t );
//uint16_t        app_afe_adc_gain_get( void );
//void            app_afe_adc_gain_set( const uint16_t );

//uint16_t        app_afe_adc_offset_get( void );
//uint16_t        app_afe_adc_offset_set( const uint16_t );


#endif //APP_H
