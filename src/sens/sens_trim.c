/**
  * @file    sens_trim.c
  * @brief   Sensor routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


//#include <stdio.h>
//#include <stdbool.h>
//#include <stdlib.h>
#include <math.h>
#include "sens.h"
//#include "bsp.h"
//#include "app.h"


/*******************************************************************************
* PRIVATE
*******************************************************************************/


/*******************************************************************************
* PUBLIC
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
void
sens_trim_restore(                              sens_trim_t *   p,
                                        const   float           t_digc )
{
                float   x       = p->oxgn_ppm[ 1].u32 - p->oxgn_ppm[ 0].u32;
                float   y       = p->oxgn_raw[ 1].u32 - p->oxgn_raw[ 0].u32;
        //const   float   k       = sens_coef_get( t_digc );
        //const   float   k       = sens_get_k_temp_digc( t_digc );
        const   float   k       = 1.0f;


        p->tg           = (x / y) * k;
        p->ofst         = (p->oxgn_ppm[ 1].u32 - (p->tg * p->oxgn_raw[ 1].u32)) * k;

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
