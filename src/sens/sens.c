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


/*
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
*/

/*******************************************************************************
* PRIVATE
*******************************************************************************/
//static
float
sens_get_k_temp_digc(                   const   float           t_digc )
{
        const   float   a       =  2.01e-06;
        const   float   b       = -2.60e-05;
        const   float   c       =  1.70e-02;
        const   float   d       =  5.61e-01;

                //uint32_t        k       = (powf(t_digc, 3) * a) + (powf(t_digc, 2) * b) + (t_digc * c) + d;
        //return( k );
        return( (powf(t_digc, 3) * a) + (powf(t_digc, 2) * b) + (t_digc * c) + d );
}


/*******************************************************************************
* PUBLIC
*******************************************************************************/
/*
uint32_t
sens_oxgn_raw_to_ppm(                   const   sens_t *        p )
{
        const   sens_oxgn_t *   oxgn    = &( p->oxgn );
        const   sens_temp_t *   temp    = &( p->temp );
        const   sens_pres_t *   pres    = &( p->pres );
        const   sens_trim_t *   trim    = &( p->trim );
        const   size_t          idx     = trim->point[ 0].timestmp.i32 > trim->point[ 1].timestmp.i32 ? 0 : 1;
        const   float           k_pres  = trim->point[idx].pres_raw.i32 / (float) pres->raw.i32;
        //const   float           k_temp  = sens_get_k_temp_digc( temp->digc.f32 );

        const   float           digc    = lps25_temperature_raw_to_digc( trim->point[idx].temp_raw );
                float           k_temp  = sens_get_k_temp_digc( digc );
                float           ppm;


        ppm     =  oxgn->raw.i32;
        ppm     *= trim->tg;
        ppm     += trim->ofst;
        ppm     /= k_temp;
        //ppm     *= k_pres;

        return( (int32_t) (ppm + 0.5) );
}
*/
/*
uint32_t
sens_oxgn_raw_to_ppm(                   const   sens_t *        p )
{
        const   sens_oxgn_t *   oxgn    = &( p->oxgn );
        const   sens_temp_t *   temp    = &( p->temp );
        const   sens_pres_t *   pres    = &( p->pres );
        const   sens_trim_t *   trim    = &( p->trim );
        const   size_t          idx     = trim->point[ 0].timestmp.i32 > trim->point[ 1].timestmp.i32 ? 0 : 1;
        const   float           k_pres  = trim->point[idx].pres_raw.i32 / (float) pres->raw.i32;
        //const   float           k_temp  = sens_get_k_temp_digc( temp->digc.f32 );

        const   float           digc    = lps25_temperature_raw_to_digc( trim->point[idx].temp_raw );
                float           k_temp  = sens_get_k_temp_digc( digc );
                float           ppm;


        k_temp  /= sens_get_k_temp_digc( temp->digc.f32 );
        ppm     = oxgn->raw.i32 * k_temp;
        //ppm     *= k_pres;
        ppm     *= trim->tg;
        ppm     += trim->ofst * k_temp;

        return( (int32_t) (ppm + 0.5) );
}
*/
/*
int32_t
sens_oxgn_raw_to_ppm(                   const   sens_t *        p )
{
        const   sens_oxgn_t *   oxgn            = &( p->oxgn );
        const   sens_temp_t *   temp            = &( p->temp );
        const   sens_pres_t *   pres            = &( p->pres );
        const   sens_trim_t *   trim            = &( p->trim );
                size_t          idx             = trim->point[ 0].timestmp.i32 > trim->point[ 1].timestmp.i32 ? 0 : 1;
                float           a               = trim->tg;
                float           b               = trim->ofst;
                float           x               = oxgn->raw.i32;
                float           cal_digc        = lps25_temperature_raw_to_digc( trim->point[idx].temp_raw );
                float           k_temp_cal      = sens_get_k_temp_digc( cal_digc );
                float           k_temp_meas     = sens_get_k_temp_digc( temp->digc.f32 );
                float           k_pres_cal      = trim->point[idx].pres_raw.i32;
                float           k_pres_meas     = pres->raw.i32;

                float           fv, ft, fp;


        fv      = a * x + b;
        ft      = k_temp_cal / k_temp_meas;
        fp      = k_pres_cal / k_pres_meas;

        APP_TRACE( "fv: %8.4f\tft: %8.4f\tfp: %8.4f\tPPM: %8.4f\n", fv, ft, fp, fv*ft*fp );


        return( (int32_t) (fv * ft * fp + 0.5) );
}
*/

int32_t
sens_oxgn_raw_to_ppm(                   const   sens_t *        p )
{
        const   sens_oxgn_t *   oxgn            = &( p->oxgn );
        const   sens_temp_t *   temp            = &( p->temp );
        const   sens_pres_t *   pres            = &( p->pres );
        const   sens_trim_t *   trim            = &( p->trim );
                float           a               = trim->tg;
                float           b               = trim->ofst;
                float           x               = oxgn->raw.i32;
                float           cal_digc        = lps25_temperature_raw_to_digc( trim->point[ 1].temp_raw );
                float           k_temp_cal      = sens_get_k_temp_digc( cal_digc );
                float           k_temp_meas     = sens_get_k_temp_digc( temp->digc.f32 );
                float           k_pres_cal      = trim->point[ 1].pres_raw.i32;
                float           k_pres_meas     = pres->raw.i32;

                float           fv, ft, fp;
                float           ppm;


        fv      = a * x + b;
        ft      = k_temp_cal / k_temp_meas;
        fp      = k_pres_cal / k_pres_meas;
        ppm     = fv * ft * fp;

        //APP_TRACE( "fv: %8.4f\tft: %8.4f\tfp: %8.4f\tPPM: %8.4f\n", fv, ft, fp, fv*ft*fp );
        //APP_TRACE( "fv: %8.4f\tft: %8.4f\tfp: %8.4f\tPPM: %8.4f\tRAW: %16d\n", fv, ft, fp, ppm, oxgn->raw.i32 );


        return( (int32_t) (ppm + 0.5) );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sens_trim_restore(                              sens_trim_t *   p,
                                        const   float           t_digc )
{
        float           p0_digc = lps25_temperature_raw_to_digc( p->point[ 0].temp_raw );
        float           k_p0    = sens_get_k_temp_digc( p0_digc );
        float           p0_ppm  = p->point[ 0].oxgn_ppm.u32;
        //float           p0_raw  = p->point[ 0].oxgn_raw.u32 / k_p0;
        float           p0_raw  = p->point[ 0].oxgn_raw.u32;

        float           p1_digc = lps25_temperature_raw_to_digc( p->point[ 1].temp_raw );
        float           k_p1    = sens_get_k_temp_digc( p1_digc );
        float           p1_ppm  = p->point[ 1].oxgn_ppm.u32;
        //float           p1_raw  = p->point[ 1].oxgn_raw.u32 / k_p1;
        float           p1_raw  = p->point[ 1].oxgn_raw.u32;


        p->tg           = (p1_ppm - p0_ppm) / (p1_raw - p0_raw);
        p->ofst         = p1_ppm - (p1_raw * p->tg);


//tg:   2.61425338E-2
//ofst: -1.1275E+2
/*
        APP_TRACE( "ppm[ 1]: %i\n", p->ppm[ 1].u32 );
        APP_TRACE( "ppm[ 0]: %i\n", p->ppm[ 0].u32 );
        APP_TRACE( "raw[ 1]: %i\n", p->raw[ 1].u32 );
        APP_TRACE( "raw[ 0]: %i\n", p->raw[ 0].u32 );
        APP_TRACE( "k:       %f\n", k );
        APP_TRACE( "-----------\n" );
        APP_TRACE( "tg:      %f\n", p->tg );
        APP_TRACE( "ofst:    %f\n", p->ofst );
*/
}
