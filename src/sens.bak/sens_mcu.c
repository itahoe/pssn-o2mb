/**
  * @file    sens_mcu.c
  * @brief   MCU Internal sensor's routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdlib.h>
#include "stm32f0xx_ll_adc.h"
#include "sens.h"


/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t
sens_mcu_raw_to_vref_mV(                const   int32_t         smpl )
{
        return( __LL_ADC_CALC_VREFANALOG_VOLTAGE( smpl, LL_ADC_RESOLUTION_12B ) );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t
sens_mcu_raw_to_celsius(                const   int32_t         smpl,
                                        const   int32_t         vref_mv )
{
        return( __LL_ADC_CALC_TEMPERATURE( vref_mv, smpl, LL_ADC_RESOLUTION_12B ) );
}
