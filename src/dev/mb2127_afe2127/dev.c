/**
  * @file    dev.c
  * @brief   Device implementation
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <string.h>
#include "dev.h"
#include "stm32.h"
#include "nvm.h"


/**
  * @brief  
  * @param  None
  * @retval None
  */
int
dev_init(                                       dev_t *         p )
{
    nvm_init();
    dev_nvm_restore( p );
    dev_sens_trim_restore( &p->sens );


    for( size_t i = 0; i < CONF_SENS_LPF_ORDER_MAX; i++ )
    {
        sma_init( &p->sens.meas.sma[ i], p->sens.meas.sma[ i].buf, CONF_SENS_LPF_FCUT_MAX );
        sma_init( &p->sens.temp.sma[ i], p->sens.temp.sma[ i].buf, CONF_SENS_LPF_FCUT_MAX );
        sma_init( &p->sens.pres.sma[ i], p->sens.pres.sma[ i].buf, CONF_SENS_LPF_FCUT_MAX );
    }

    return( 0 );
}
