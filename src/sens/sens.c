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
* PRIVATE
*******************************************************************************/
/*
static
float
sens_get_k_temp_lps25_raw(              const   uint16_t        t_raw )
{
        #if SENS_OXGN_T_COEF_DISABLE > 0
        return( 1 );
        #else
        const   float   t_digc  = lps25_temperature_raw_to_digc( t_raw );
        const   float   a       =  2.01e-06;
        const   float   b       = -2.60e-05;
        const   float   c       =  1.70e-02;
        const   float   d       =  5.61e-01;
        return( (powf(t_digc, 3) * a) + (powf(t_digc, 2) * b) + (t_digc * c) + d );
        #endif
}
*/

/*******************************************************************************
* PUBLIC
*******************************************************************************/
/*
float
sens_get_k_temp_digc(                   const   float           t_digc )
{
        #if SENS_OXGN_T_COEF_DISABLE > 0
        return( 1 );
        #else
        const   float   a       =  2.01e-06;
        const   float   b       = -2.60e-05;
        const   float   c       =  1.70e-02;
        const   float   d       =  5.61e-01;
        return( (powf(t_digc, 3) * a) + (powf(t_digc, 2) * b) + (t_digc * c) + d );
        #endif
}
*/

/**
  * @brief  
  * @param  None
  * @retval None
  */
/*
int32_t
sens_oxgn_raw_to_ppm(                   const   sens_t *        p )
{
        const   sens_oxgn_t *   oxgn    = &( p->oxgn );
        const   sens_trim_t *   trim    = &( p->trim );
        //const   sens_avrg_t *   avrg    = &( p->avrg );
        const   sens_temp_t *   temp    = &( p->temp );
        //const   float           k       = sens_get_k_temp_digc( temp->digc.f32 );
        const   float           k       = sens_get_k_temp_raw_ad7799( temp->raw.u32 );
        const   float           ppm     = (oxgn->raw.i32 * trim->tg) + trim->ofst;

        return( (int32_t) ((ppm / k) + 0.5) );
}
*/



static
float
sens_get_k_temp(                        const   uint32_t        t_trim_raw,
                                        const   uint32_t        t_mes_raw )
{
        #if SENS_OXGN_T_COEF_DISABLE > 0
        return( 1 );
        #else
        const   float   t_trim_digc     = lps25_temperature_raw_to_digc( t_trim_raw );
        const   float   t_mes_digc      = lps25_temperature_raw_to_digc( t_mes_raw );
        const   float   a               =  2.01e-06;
        const   float   b               = -2.60e-05;
        const   float   c               =  1.70e-02;
        const   float   d               =  5.61e-01;

        const   float   k_trim_digc     = (powf(t_trim_digc, 3) * a) + (powf(t_trim_digc, 2) * b) + (t_trim_digc* c) + d;
        const   float   k_mes_digc      = (powf(t_mes_digc, 3) * a) + (powf(t_mes_digc, 2) * b) + (t_mes_digc * c) + d;

        return( k_trim_digc/k_mes_digc );
        #endif
}


static
float
sens_get_k_adj(                         const   uint32_t        t_trim_raw,
                                        const   uint32_t        t_mes_raw )
{
        //const   float           k_adj   = ( trim->temp_raw[idx].u32, temp->raw.u32 );
        const   float   t_trim_digc     = lps25_temperature_raw_to_digc( t_trim_raw );
        const   float   t_mes_digc      = lps25_temperature_raw_to_digc( t_mes_raw );

        return( 1 + (t_trim_digc - t_mes_digc) / -290.0f );
}


int32_t
sens_oxgn_raw_to_ppm(                   const   sens_t *        p )
{
        const   sens_oxgn_t *   oxgn    = &( p->oxgn );
        const   sens_temp_t *   temp    = &( p->temp );
        const   sens_pres_t *   pres    = &( p->pres );
        const   sens_trim_t *   trim    = &( p->trim );
        const   size_t          idx     = trim->timestmp[ 0].i32 > trim->timestmp[ 1].i32 ? 0 : 1;
        //const   float           k_temp  = 1;
        //const   float           k_temp  = sens_get_k_temp_lps25_raw( temp->raw.u32 );
        //const   float           k_temp  = trim->temp_raw[idx].i32 / (float) temp->raw.u32;

                float           ppm;
        const   float           k_pres  = trim->pres_raw[idx].i32 / (float) pres->raw.i32;
        const   float           k_temp  = sens_get_k_temp( trim->temp_raw[idx].u32, temp->raw.u32 );
        const   float           k_adj   = sens_get_k_adj( trim->temp_raw[idx].u32, temp->raw.u32 );


        ppm     = (oxgn->raw.i32 * trim->tg) + trim->ofst;
        ppm     *= k_pres;
        ppm     *= k_temp;
        ppm     *= k_adj;

        return( (int32_t) (ppm + 0.5) );
}
