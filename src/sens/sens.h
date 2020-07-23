/**
  * @file    sens.h
  * @brief   Sensor's block header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef SENS_H
#define SENS_H


#include <stdint.h>
#include "config.h"


typedef uint16_t (* sens_mcu_func_t) (const size_t idx);


typedef struct  sens_avrg_s
{
        int32_t *               buf;
        size_t                  buf_sizeof;
        size_t                  idx;
        int32_t                 sum;
} sens_avrg_t;


typedef union
{
        int32_t                 i32;
        uint32_t                u32;
        uint16_t                u16[ 2];
} sens_union_t;


typedef struct  sens_oxgn_s
{
        sens_union_t            ppm;
        int32_t                 avrg;
        uint16_t                instability;
        uint16_t                offset;
} sens_oxgn_t;


typedef struct  sens_trim_s
{
        sens_union_t            raw[ CFG_SENS_TRIM_NUM_POINTS ];
        sens_union_t            ppm[ CFG_SENS_TRIM_NUM_POINTS ];
        sens_union_t            timestamp[ CFG_SENS_TRIM_NUM_POINTS ];
        float                   ofst;
        float                   tg;
} sens_trim_t;


typedef struct  sens_lm75_s
{
        int32_t                 celsius;
} sens_lm75_t;


typedef struct  sens_mcu_s
{
        int32_t                 celsius;
        int32_t                 vref_mV;
        sens_mcu_func_t         serial_num;
        sens_mcu_func_t         device_id;
        sens_mcu_func_t         hardware_id;
        sens_mcu_func_t         firmware_id;
        uint16_t                starts_cnt;
} sens_mcu_t;


typedef struct  sens_s
{
        sens_oxgn_t             oxgn;
        sens_trim_t             trim;
        sens_lm75_t             lm75;
        sens_mcu_t              mcu;
        sens_avrg_t             avrg;
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


int32_t
sens_oxgn_read( void );


void
sens_oxgn_run(                          const   uint16_t *      data,
                                        const   size_t          len );

int32_t
sens_oxgn_raw_avrg(                             sens_avrg_t *   p,
                                                int32_t         sample );


int32_t
sens_oxgn_raw_to_ppm(                   const   sens_trim_t *   p,
                                        const   int32_t         raw,
                                        const   int32_t         t );

int32_t
sens_oxgn_avrg_to_ppm(                  const   sens_trim_t *   p,
                                        const   int32_t         avrg,
                                        const   int32_t         avrg_sizeof,
                                        const   int32_t         t );

int32_t
sens_oxgn_raw_avrg(                             sens_avrg_t *   p,
                                                int32_t         sample );


int32_t
sens_oxgn_get_instability(                      sens_avrg_t *   p,
                                                int32_t         sample );


/*******************************************************************************
* SENS OXGN OFST
*******************************************************************************/
void
sens_oxgn_ofst_init(                    const   size_t          samplerate_sps );

void
sens_oxgn_ofst_run( void );

void
sens_oxgn_ofst_set(                     const   uint16_t        raw16 );

uint16_t
sens_oxgn_ofst_get( void );


/*******************************************************************************
* SENS MCU
*******************************************************************************/
int32_t
sens_mcu_raw_to_vref_mV(                const   int32_t         smpl );

int32_t
sens_mcu_raw_to_celsius(                const   int32_t         smpl,
                                        const   int32_t         vref_mv );


#endif  //SENS_H
