/**
  * @file    dev_sens_temp.c
  * @brief   Device Sensor Temperature
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <string.h>
#include "dev.h"
#include "lps25.h"
#include "sma.h"


/**
  * @brief  
  * @param  None
  * @retval None
  */
int
dev_sens_temp_update(                           dev_sens_t *    p )
{
    sma_data_t      raw     = lps25_get_temperature_raw();

/*
    for( size_t i = 0; i < p->lpf.order; i++ )
    {
        if( i < CONF_SENS_LPF_ORDER_MAX )
        {
            raw = sma_filter( &p->meas.sma[ i], raw );
        }
        else
        {
            break;
        }
    }
*/

    p->temp.raw         = raw;
    p->temp.digc.f32    = lps25_temperature_raw_to_digc( (int16_t) raw );


    return( 0 );
}
