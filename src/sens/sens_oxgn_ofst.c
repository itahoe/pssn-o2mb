/**
  * @file    sens_oxgn_ofst.c
  * @brief   Oxygen sensor offset routines
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

