/**
  * @file    dev_sens_meas.c
  * @brief   Device Sensor Measure
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <string.h>
#include <math.h>
#include "dev.h"
#include "ad7799.h"


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
static
float
sens_get_k_temp_digc(                   const   float           t_digc )
{
        const   float   a       =  2.01e-06;
        const   float   b       = -2.60e-05;
        const   float   c       =  1.70e-02;
        const   float   d       =  5.61e-01;
                float   k;

        k       = (powf(t_digc, 3) * a) + (powf(t_digc, 2) * b) + (t_digc * c) + d;

        return(k);
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
static
float
dev_sens_meas_raw_to_ppm(                       dev_sens_t *        p,
                                                int32_t             raw )
{
        //const   sens_oxgn_t *   oxgn            = &( p->oxgn );
        //const   dev_sens_trim_t *   trim            = &( p->trim );

        //float           k_temp  = 1.0;
        //float           k_temp  = sens_get_k_temp_digc( p->temp.digc.f32 );

        float           kt_trim = sens_get_k_temp_digc( p->span.temp_digc.f32 );
        float           kt_meas = sens_get_k_temp_digc( p->temp.digc.f32 );
        float           kt      = kt_trim / kt_meas;
        //float           kt      = 1.0f;

        float           pres_trim = p->span.pres_raw.u32;
        float           pres_meas = p->pres.raw.u32;
        float           kp      = pres_trim / pres_meas;
        //float           kp      = 1.0f;

        float           ppm     = (raw + p->offset) * p->slope * kt * kp;


        return(ppm);
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
dev_sens_trim_restore(                      dev_sens_t *   p )
{
        float   zero_raw        = p->zero.raw.i32;
        float   span_raw        = p->span.raw.i32;
        float   span_ppm        = p->span.ppm.i32;

        p->offset       = 0.0f - zero_raw;
        p->slope        = span_ppm / (span_raw + p->offset);
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int
dev_sens_meas_update(                           dev_sens_t *    p )
{
    int32_t     raw;


    raw             = ad7799_get_sample();
    p->meas.raw.i32 = raw;

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

    p->meas.ppm.f32  = dev_sens_meas_raw_to_ppm( p, raw );


    return( 0 );
}
