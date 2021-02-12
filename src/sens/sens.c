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
#include "bsp.h"
#include "app.h"
#include "lps25.h"


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
//static
float
sens_get_k_temp_digc(                   const   float           t_digc )
{
        const   float   a       =  2.01e-06;
        const   float   b       = -2.60e-05;
        const   float   c       =  1.70e-02;
        const   float   d       =  5.61e-01;

        return( (powf(t_digc, 3) * a) + (powf(t_digc, 2) * b) + (t_digc * c) + d );
}


/*******************************************************************************
* PUBLIC
*******************************************************************************/
/*******************************************************************************
*       example:
*       signal @ T[у]=signal @ 25у x Pcoe @ T[у]
*       signal @ (0у)=signal @ 25у x 0.561
*******************************************************************************/

float
sens_oxgn_raw_to_ppm(                           sens_t *        p )
{
                sens_oxgn_t *   oxgn            = &( p->oxgn );
        const   sens_temp_t *   temp            = &( p->temp );
        const   sens_pres_t *   pres            = &( p->pres );
        const   sens_trim_t *   trim            = &( p->trim );
                float           a               = trim->tg;
                float           b               = trim->ofst;
                float           x               = oxgn->raw.i32;
                //float           cal_digc        = temp->digc.f32;
                //float           cal_digc        = lps25_temperature_raw_to_digc( trim->point[ 1].temp_raw );
                float           cal_digc        = trim->point[ 1].temp_digc.f32;
                float           k_temp_cal      = sens_get_k_temp_digc( cal_digc );
                float           k_temp_meas     = sens_get_k_temp_digc( temp->digc.f32 );
                float           k_pres_cal      = trim->point[ 1].pres_raw.i32;
                float           k_pres_meas     = pres->raw.i32;

                float           ppm;


        oxgn->fv.f32    = a * x + b;
        oxgn->ft.f32    = k_temp_cal / k_temp_meas;
        oxgn->fp.f32    = k_pres_cal / k_pres_meas;
        ppm             = oxgn->fv.f32 * oxgn->ft.f32 * oxgn->fp.f32;

        return( ppm );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sens_trim_restore(                              sens_trim_t *   p )
{
        //float           p0_digc = lps25_temperature_raw_to_digc( p->point[ 0].temp_raw );
        float           p0_digc = p->point[ 0].temp_digc.f32;
        float           k_p0    = sens_get_k_temp_digc( p0_digc );
        float           p0_ppm  = p->point[ 0].oxgn_ppm.u32;
        //float           p0_raw  = p->point[ 0].oxgn_raw.u32 / k_p0;
        float           p0_raw  = p->point[ 0].oxgn_raw.u32;

        //float           p1_digc = lps25_temperature_raw_to_digc( p->point[ 1].temp_raw );
        float           p1_digc = p->point[ 1].temp_digc.f32;
        float           k_p1    = sens_get_k_temp_digc( p1_digc );
        float           p1_ppm  = p->point[ 1].oxgn_ppm.u32;
        //float           p1_raw  = p->point[ 1].oxgn_raw.u32 / k_p1;
        float           p1_raw  = p->point[ 1].oxgn_raw.u32;


        p->tg           = (p1_ppm - p0_ppm) / (p1_raw - p0_raw);
        p->ofst         = p1_ppm - (p1_raw * p->tg);
}
