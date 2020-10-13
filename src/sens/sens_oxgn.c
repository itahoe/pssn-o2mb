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


/*******************************************************************************
* PRIVATE
*******************************************************************************/
/*
static
float
//sens_coef_get(                          const   float           t_digc )
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
/*
int32_t
sens_oxgn_read( void )
{
        return( bsp_oxgn_read() );
}
*/

/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sens_oxgn_stream_run(                   const   uint16_t *      data,
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

        p->buf[ p->idx++ ]      =  sample;

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
/*
int32_t
sens_oxgn_raw_to_ppm(                   const   sens_trim_t *           p,
                                        const   int32_t                 raw,
                                        const   int32_t                 t_digc )
{
        const   float   k       = sens_get_k_temp_digc( t_digc );
        const   float   ppm     = (raw * p->tg) + p->ofst;
        return( (int32_t) ((ppm / k) + 0.5) );
}
*/

/**
  * @brief  
  * @param  None
  * @retval None
  */
/*
int32_t
sens_oxgn_avrg_to_ppm(                  const   sens_trim_t *           p,
                                        const   int32_t                 avrg,
                                        const   int32_t                 avrg_sizeof,
                                        const   float                   t_digc )
{
        const   float   k       = sens_get_k_temp_digc( t_digc );
        const   float   ppm     = (avrg * p->tg) / avrg_sizeof + p->ofst;
        return( (int32_t) ( (float) (ppm / k) + 0.5) );
}
*/

