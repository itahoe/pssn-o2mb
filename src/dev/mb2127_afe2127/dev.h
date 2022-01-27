#ifndef DEV_H
#define DEV_H


#include <stdbool.h>
#include "ad7799.h"
#include "nvm.h"
#include "sma.h"
#include "config.h"


//#define DEV_SMA_ORDER                   8
//#define DEV_SMA_BUF_SIZEOF              16



typedef struct  dev_conf_s
{
        uint16_t        device_id;
        uint16_t        hardware_id;
        uint16_t        firmware_id[ 2];
        uint16_t        serial_num[ 8];
        uint16_t        adc_vref_mV;
        uint16_t        adc_resolution_bits;
} dev_conf_t;


typedef union
{
        int32_t                 i32;
        uint32_t                u32;
        uint16_t                u16[ 2];
        float                   f32;
} dev_union_t;


typedef struct  dev_evt_s
{
        bool                    mcu_systick_1hz;
        bool                    mcu_adc;
        bool                    mcu_ser1_recv;
} dev_evt_t;


typedef struct  dev_afe_s
{
        uint16_t                bias;
} dev_afe_t;


typedef struct  dev_mcu_s
{
        int32_t                 degc;
        int32_t                 vref_mV;
} dev_mcu_t;


/*
typedef struct  dev_sens_sma_s
{
        uint32_t                buf[ DEV_SMA_ORDER ][ DEV_SMA_BUF_SIZEOF ];
        //uint32_t *              buf;
        size_t                  buf_sizeof;
        size_t                  idx;
        int32_t                 sum;
        uint16_t                slope;
} dev_sens_sma_t;
*/


typedef struct  dev_trim_s
{
        dev_union_t             timestmp;
        dev_union_t             ppm;
        dev_union_t             raw;
        int16_t                 temp_raw;
        dev_union_t             temp_digc;
        dev_union_t             pres_raw;
} dev_trim_t;


typedef struct  dev_sens_meas_s
{
    dev_union_t             raw;
    dev_union_t             ppm;
    uint16_t                rmse;
    sma_t                   sma[CONF_SENS_LPF_ORDER_MAX];
} dev_sens_meas_t;


typedef struct  dev_sens_temp_s
{
        int32_t                 raw;
        dev_union_t             digc;
        sma_t                   sma[CONF_SENS_LPF_ORDER_MAX];
} dev_sens_temp_t;


typedef struct  dev_sens_pres_s
{
        dev_union_t             raw;
        dev_union_t             hPa;
        sma_t                   sma[CONF_SENS_LPF_ORDER_MAX];
} dev_sens_pres_t;


typedef struct  dev_sens_lpf_s
{
    uint16_t                    fcut;
    uint16_t                    order;
} dev_sens_lpf_t;


typedef struct  dev_sens_s
{
    dev_sens_temp_t             temp;
    dev_sens_pres_t             pres;
    dev_sens_meas_t             meas;
    dev_sens_lpf_t              lpf;
    dev_union_t                 drift_k_temp;
    dev_union_t                 drift_k_pres;
    dev_trim_t                  zero;
    dev_trim_t                  span;
    float                       offset;
    float                       slope;
} dev_sens_t;


typedef struct  dev_s
{
        uint16_t                starts_cnt;
        dev_conf_t const *      conf;
        dev_evt_t               evt;
        dev_mcu_t               mcu;
        dev_afe_t               afe;
        ad7799_t                ad7799;
        dev_sens_t              sens;
} dev_t;


/*******************************************************************************
* DEV
*******************************************************************************/
int
dev_init(                                       dev_t *         p );

/*******************************************************************************
* DEV AFE
*******************************************************************************/
uint16_t
dev_afe_bias_get(                       const   dev_t *         p );

void
dev_afe_bias_set(                               dev_t *         p,
                                        const   uint16_t        data );

/*******************************************************************************
* DEV AFE ADC
*******************************************************************************/
uint16_t
dev_afe_adc_mode_get(                   const   dev_t *         p );

void
dev_afe_adc_mode_set(                           dev_t *         p,
                                        const   uint16_t        val );

uint16_t
dev_afe_adc_conf_get(                   const   dev_t *         p );

void
dev_afe_adc_conf_set(                           dev_t *         p,
                                        const   uint16_t        val );

int
dev_afe_adc_psw_get(                            dev_t *         p );

void
dev_afe_adc_psw_set(                            dev_t *         p,
                                        const   int             val );

int
dev_afe_adc_unipolar_get(               const   dev_t *         p );

void
dev_afe_adc_unipolar_set(                       dev_t *         p,
                                        const   int             val );

int
dev_afe_adc_buffer_enable_get(          const   dev_t *         p );

void
dev_afe_adc_buffer_enable_set(                  dev_t *         p,
                                        const   int             val );

uint16_t
dev_afe_adc_chnl_get(                   const   dev_t *         p );

void
dev_afe_adc_chnl_set(                           dev_t *         p,
                                        const   uint16_t        channel );

uint16_t
dev_afe_adc_gain_get(                   const   dev_t *         p );

void
dev_afe_adc_gain_set(                           dev_t *         p,
                                        const   uint16_t        gain );


/*******************************************************************************
* DEV SENS
*******************************************************************************/
void
dev_sens_trim_restore(                          dev_sens_t *            p );

float
dev_sens_meas_raw_to_ppm(                       dev_sens_t *            p,
                                                int32_t                 raw );



/*******************************************************************************
* DEV MEAS
*******************************************************************************/
int
dev_sens_meas_update(                           dev_sens_t *        p );

/*******************************************************************************
* DEV TEMP
*******************************************************************************/
int
dev_sens_temp_update(                           dev_sens_t *        p );

/*******************************************************************************
* DEV PRES
*******************************************************************************/
int
dev_sens_pres_update(                           dev_sens_t *        p );


/*******************************************************************************
* DEV NVM
*******************************************************************************/
void
dev_nvm_write(                                  nvm_addr_t      addr,
                                    const       uint16_t        data );

uint16_t
dev_nvm_read(                       const       nvm_addr_t      addr );

void
dev_nvm_reset(                                  dev_t *         p );

void
dev_nvm_restore(                                dev_t *         p );


#endif //DEV_H
