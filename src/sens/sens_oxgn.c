/**
  * @file    sens_oxgn.c
  * @brief   Oxygen sensor routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "sens.h"
#include "bsp.h"
#include "app.h"


#define SENS_OXGN_T_COEF_ENABLE         0


/*******************************************************************************
* STATIC FUNCTIONS
*******************************************************************************/
static
float
sens_coef_get(                          const   int32_t                 t )
{
        #if SENS_OXGN_T_COEF_ENABLE > 0
        const   float   a       =  2.01e-06;
        const   float   b       = -2.60e-05;
        const   float   c       =  1.70e-02;
        const   float   d       =  5.61e-01;
        return( (powf( t, 3) * a) + (powf( t, 2) * b) + ( t * c) + d );
        #else
        return( 1 );
        #endif
}


/*******************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sens_oxgn_init(                         const   size_t          samplerate_sps )
{
        bsp_oxgn_init( samplerate_sps );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sens_oxgn_run(                          const   uint16_t *      data,
                                        const   size_t          len )
{
        bsp_oxgn_run( data, len );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t
sens_oxgn_raw_avrg(                             sens_avrg_t *           p,
                                                int32_t                 sample )
{
        p->sum                  -= p->buf[ p->idx ];
        p->sum                  += sample;
        p->buf[ p->idx ]        =  sample;
        p->idx++;

        if( p->idx >= p->buf_sizeof )
        {
                p->idx                  = 0;
        }

        return( p->sum / p->buf_sizeof );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t
sens_oxgn_get_instability(                      sens_avrg_t *           p,
                                                int32_t                 sample )
{
        int32_t         diff    = 0;


        for( int i = 0; i < p->buf_sizeof; i++ )
        {
                diff    += abs( ( sample - p->buf[ i ] ) );
        }

        return( diff / p->buf_sizeof );
}


/*******************************************************************************
* CALIBRATE
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sens_trim_restore(                              sens_trim_t *           p )
{
        float   x       = p->ppm[ 1].u32 - p->ppm[ 0].u32;
        float   y       = p->raw[ 1].u32 - p->raw[ 0].u32;
        p->tg           = x / y;
        p->ofst         = p->ppm[ 1].u32 - (p->tg * p->raw[ 1].u32);
}


/*******************************************************************************
        example:
        signal @ T[у]=signal @ 25у x Pcoe @ T[у]
        signal @ (0у)=signal @ 25у x 0.561
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t
sens_oxgn_raw_to_ppm(                   const   sens_trim_t *           p,
                                        const   int32_t                 raw,
                                        const   int32_t                 t )
{
        const   float   k       = sens_coef_get( t );
        const   float   ppm     = (raw * p->tg) + p->ofst;


        return( (int32_t) ((ppm / k) + 0.5) );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t
sens_oxgn_avrg_to_ppm(                  const   sens_trim_t *           p,
                                        const   int32_t                 avrg,
                                        const   int32_t                 avrg_sizeof,
                                        const   int32_t                 t )
{
        const   float   k       = sens_coef_get( t );
        const   float   ppm     = (avrg * p->tg) / avrg_sizeof + p->ofst;


        return( (int32_t) ((ppm / k) + 0.5) );
}


/*******************************************************************************
* OXGN OFST
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sens_oxgn_ofst_init(                    const   size_t          samplerate_sps )
{
        bsp_oxgn_ofst_init( samplerate_sps );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sens_oxgn_ofst_run( void )
{
        bsp_oxgn_ofst_run();
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sens_oxgn_ofst_set(                     const   uint16_t        raw16 )
{
        bsp_oxgn_ofst_set( raw16 );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
sens_oxgn_ofst_get( void )
{
        return( bsp_oxgn_ofst_get() );
}

