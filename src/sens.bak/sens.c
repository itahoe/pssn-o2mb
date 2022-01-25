/**
  * @file    sens.c
  * @brief   Sensor routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "sens.h"
//#include "stm32.h"
#include "app.h"
#include "lps25.h"


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
/*
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
*/

/*******************************************************************************
* PUBLIC
*******************************************************************************/
/*******************************************************************************
*       example:
*       signal @ T[у]=signal @ 25у x Pcoe @ T[у]
*       signal @ (0у)=signal @ 25у x 0.561
*******************************************************************************/
/*
float
sens_oxgn_raw_to_ppm(                           sens_t *        p )
{
                sens_oxgn_t *   oxgn            = &( p->oxgn );
        //const   sens_temp_t *   temp            = &( p->temp );
        //const   sens_pres_t *   pres            = &( p->pres );
        const   sens_trim_t *   trim            = &( p->trim );
                float           cal_digc        = trim->point[ 1].temp_digc.f32;
                float           k_temp_cal      = sens_get_k_temp_digc( cal_digc );
                //float           k_temp_meas     = sens_get_k_temp_digc( temp->digc.f32 );
                float           k_temp_meas     = 1;

        return( (oxgn->raw.i32 + trim->offset) * trim->slope * k_temp_meas );
}
*/




/*
float
sens_oxgn_raw_to_ppm(                           sens_t *        p,
                                                int32_t         raw )
{
        //const   sens_oxgn_t *   oxgn            = &( p->oxgn );
        const   sens_trim_t *   trim            = &( p->trim );

        //float           k_temp  = 1.0;
        //float           k_temp  = sens_get_k_temp_digc( p->temp.digc.f32 );

        float           kt_trim = sens_get_k_temp_digc( p->trim.point[ 1].temp_digc.f32 );
        float           kt_meas = sens_get_k_temp_digc( p->temp.digc.f32 );
        float           kt      = kt_trim / kt_meas;
        //float           kt      = 1.0f;

        float           pres_trim = p->trim.point[ 1].pres_raw.u32;
        float           pres_meas = p->pres.raw.u32;
        float           kp      = pres_trim / pres_meas;
        //float           kp      = 1.0f;

        float           ppm     = (raw + trim->offset) * trim->slope * kt * kp;


        return(ppm);
}
*/

/**
  * @brief  
  * @param  None
  * @retval None
  */
/*
void
sens_trim_restore(                              sens_trim_t *   p )
{
        float   raw_zero        = p->point[ 0].oxgn_raw.u32;
        float   raw_span        = p->point[ 1].oxgn_raw.u32;
        float   ppm_span        = p->point[ 1].oxgn_ppm.u32;

        p->offset       = 0.0 - raw_zero;
        //p->offset       = 0.0 - (raw_zero * ktemp_zero);

        p->slope        = ppm_span / (raw_span + p->offset);
        //p->slope        = ppm_span / ((raw_span * ktemp_span) + p->offset);
}
*/
