/**
  * @file    dev_sens_pres.c
  * @brief   Device Sensor Pressure
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
dev_sens_pres_update(                           dev_sens_t *    p )
{
    sma_data_t      raw = lps25_get_pressure_raw();

    for( size_t i = 0; i < p->lpf.order; i++ )
    {
        if( i < CONF_LPF_ORDER_MAX )
        {
            raw = sma_filter( &p->meas.sma[ i], raw );
        }
        else
        {
            break;
        }
    }

    p->pres.raw.i32       = raw;
    p->pres.hPa.f32       = lps25_pressure_raw_to_hpa( raw );


    return( 0 );
}
