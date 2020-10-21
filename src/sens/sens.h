/**
  * @file    sens.h
  * @brief   Sensor's block header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef SENS_H
#define SENS_H


#include <stdint.h>
#include <stdio.h>
#include "config.h"


typedef uint16_t (* sens_mcu_func_t) (const size_t idx);


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
        uint32_t                sum;
        uint16_t                slope;
} sens_avrg_t;

typedef struct  sens_trim_point_s
{
        sens_union_t            timestmp;
        sens_union_t            oxgn_ppm;
        sens_union_t            oxgn_raw;
        int16_t                 temp_raw;
        sens_union_t            pres_raw;
} sens_trim_point_t;

typedef struct  sens_trim_s
{
        //sens_union_t            timestmp[ CFG_SENS_TRIM_NUM_POINTS ];
        //sens_union_t            oxgn_ppm[ CFG_SENS_TRIM_NUM_POINTS ];
        //sens_union_t            oxgn_raw[ CFG_SENS_TRIM_NUM_POINTS ];
        //int16_t                 temp_raw[ CFG_SENS_TRIM_NUM_POINTS ];
        //sens_union_t            pres_raw[ CFG_SENS_TRIM_NUM_POINTS ];
        sens_trim_point_t       point[ CFG_SENS_TRIM_NUM_POINTS ];
        float                   ofst;
        float                   tg;
} sens_trim_t;


typedef struct  sens_oxgn_s
{
        sens_union_t            raw;
        sens_union_t            ppm;
        uint16_t                offset;
} sens_oxgn_t;


typedef struct  sens_mcu_s
{
        int32_t                 degc;
        int32_t                 vref_mV;
        sens_mcu_func_t         serial_num;
        sens_mcu_func_t         device_id;
        sens_mcu_func_t         hardware_id;
        sens_mcu_func_t         firmware_id;
        uint16_t                starts_cnt;
} sens_mcu_t;


typedef struct  sens_temp_s
{
        //sens_union_t            raw;
        int16_t                 raw;
        sens_union_t            digc;
        //sens_avrg_t             avrg;
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
        sens_mcu_t              mcu;
        sens_avrg_t             avrg;
        sens_oxgn_t             oxgn;
        sens_temp_t             temp;
        sens_pres_t             pres;
} sens_t;


/*******************************************************************************
* 
*******************************************************************************/
void
sens_trim_restore(                              sens_trim_t *           p,
                                        const   float                   t_cels );


/*******************************************************************************
* SENS
*******************************************************************************/
/*
float
sens_get_k_temp_digc(                   const   float           t_digc );
*/

/*******************************************************************************
* SENS OXGN
*******************************************************************************/
void
sens_oxgn_init(                         const   size_t          samplerate_sps );
/*
int32_t
sens_oxgn_read( void );
*/

/*******************************************************************************
* SENS OXGN STREAM
*******************************************************************************/
void
sens_oxgn_stream_run(                   const   uint16_t *      data,
                                        const   size_t          len );


/*******************************************************************************
* SENS OXGN CONVERSION
*******************************************************************************/
/*
uint32_t
sens_oxgn_raw_avrg(                             sens_avrg_t *   p,
                                                uint32_t        sample );
*/

/*
int32_t
sens_oxgn_avrg_to_ppm(                  const   sens_trim_t *   p,
                                        const   int32_t         avrg,
                                        const   int32_t         avrg_sizeof,
                                        const   float           t_cels );
*/

uint32_t
sens_oxgn_raw_to_ppm(                   const   sens_t *        p );

uint32_t
sens_oxgn_raw_avrg(                             sens_avrg_t *   p,
                                                uint32_t        sample );


int32_t
sens_oxgn_get_slope(                            sens_avrg_t *   p,
                                                int32_t         sample );


/*******************************************************************************
* SENS OXGN OFST
*******************************************************************************/
/*
void
sens_oxgn_ofst_init(                    const   size_t          samplerate_sps );

void
sens_oxgn_ofst_run( void );

void
sens_oxgn_ofst_set(                     const   uint16_t        raw16 );

uint16_t
sens_oxgn_ofst_get( void );
*/

/*******************************************************************************
* SENS MCU
*******************************************************************************/
int32_t
sens_mcu_raw_to_vref_mV(                const   int32_t         smpl );

int32_t
sens_mcu_raw_to_celsius(                const   int32_t         smpl,
                                        const   int32_t         vref_mv );


#endif  //SENS_H
