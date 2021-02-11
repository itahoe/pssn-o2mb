/**
  * @file    sens_average.c
  * @brief   Average routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdlib.h>
#include "sens.h"


/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t
sens_average(                                   sens_avrg_t *           p,
                                                int32_t                 sample )
{
        int32_t         average;


        p->sum                  -= p->buf[ p->idx ];
        p->sum                  += sample;

        p->buf[ p->idx++ ]      =  sample;

        if( p->idx >= p->buf_sizeof )
        {
                p->idx                  = 0;
        }

        average = p->sum / p->buf_sizeof;

        return( average );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t
sens_oxgn_get_slope(                            sens_avrg_t *           p,
                                                int32_t                 sample )
{
        int32_t         diff    = 0;


        for( int i = 0; i < p->buf_sizeof; i++ )
        {
                diff    += abs( ( sample - p->buf[ i ] ) );
        }

        return( diff / p->buf_sizeof );
}
