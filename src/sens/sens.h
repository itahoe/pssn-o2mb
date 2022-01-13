/**
  * @file    sens.h
  * @brief   Sensor's block header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef SENS_H
#define SENS_H


#include <stdint.h>
#include "config.h"


typedef union
{
        int32_t                 i32;
        uint32_t                u32;
        uint16_t                u16[ 2];
        float                   f32;
} sens_union_t;


typedef struct  sens_avrg_s
{
        uint32_t *              buf;
        size_t                  buf_sizeof;
        size_t                  idx;
        int32_t                 sum;
        uint16_t                slope;
} sens_avrg_t;

typedef struct  sens_trim_point_s
{
        sens_union_t            timestmp;
        sens_union_t            oxgn_ppm;
        sens_union_t            oxgn_raw;
        int16_t                 temp_raw;
        sens_union_t            temp_digc;
        sens_union_t            pres_raw;
} sens_trim_point_t;

typedef struct  sens_trim_s
{
        sens_trim_point_t       point[ CFG_SENS_TRIM_NUM_POINTS ];
        float                   offset;
        float                   slope;
} sens_trim_t;


typedef struct  sens_oxgn_s
{
        sens_union_t            raw;
        sens_union_t            ppm;
        //uint16_t                offset;
        sens_union_t            fv;
        sens_union_t            ft;
        sens_union_t            fp;
} sens_oxgn_t;


typedef struct  sens_temp_s
{
        int32_t                 raw;
        sens_union_t            digc;
} sens_temp_t;


typedef struct  sens_pres_s
{
        sens_union_t            raw;
        sens_union_t            hPa;
        sens_avrg_t             avrg;
} sens_pres_t;


typedef struct  sens_s
{
        sens_trim_t             trim;
        //sens_afe_t              afe;
        sens_avrg_t             avrg;
        sens_oxgn_t             oxgn;
        sens_temp_t             temp;
        sens_pres_t             pres;
        sens_union_t            drift_k_temp;
        sens_union_t            drift_k_pres;
} sens_t;


/*******************************************************************************
* 
*******************************************************************************/
void
sens_trim_restore(                              sens_trim_t *   p );

/*******************************************************************************
* SENS OXGN
*******************************************************************************/
void
sens_oxgn_init(                         const   size_t          samplerate_sps );

/*******************************************************************************
* SENS OXGN CONVERSION
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
static
uint32_t
sens_correction(                        const   uint32_t        sample,
                                        const   float           temp )
{
        float           t       = temp * 0.05;
        float           f       = sample * t;
        
        return( (uint32_t) f );
}


float
sens_oxgn_raw_to_ppm(                           sens_t *        p );

int32_t
sens_average(                                   sens_avrg_t *   p,
                                                int32_t         sample );


int32_t
sens_oxgn_get_slope(                            sens_avrg_t *   p,
                                                int32_t         sample );


#endif  //SENS_H
