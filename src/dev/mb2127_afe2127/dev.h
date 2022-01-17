#ifndef DEV_H
#define DEV_H


#include <stdbool.h>
#include "ad7799.h"


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



/*
typedef struct  dev_mcu_i2c_s
{
        int                     dummy;
} dev_mcu_i2c_t;
*/
/*
typedef struct  dev_mcu_spi_s
{
        ad7799_t                ad7799;
} dev_mcu_spi_t;
*/

typedef struct  dev_mcu_s
{
        //dev_mcu_i2c_t           i2c;
        //dev_mcu_spi_t           spi;

        //size_t                  systick_1hz;
        int32_t                 degc;
        int32_t                 vref_mV;
} dev_mcu_t;

/*
typedef struct  dev_meas_temp_s
{
        int16_t                 temp_raw;
        sens_union_t            temp_digc;
} dev_meas_temp_t;
*/

typedef struct  dev_meas_sens_s
{
        dev_union_t             raw;
        dev_union_t             ppm;
} dev_meas_sens_t;


typedef struct  dev_meas_s
{
        dev_meas_sens_t         sens;
} dev_meas_t;


typedef struct  dev_s
{
        uint16_t                starts_cnt;
        dev_conf_t const *      conf;
        dev_evt_t               evt;
        //size_t                  mcu_systick_1hz;
        dev_mcu_t               mcu;
        dev_afe_t               afe;
        ad7799_t                ad7799;
        dev_meas_t              meas;

} dev_t;


/*******************************************************************************
* DEV
*******************************************************************************/
int
dev_init(                               const   dev_t *         p );

int
dev_proc(                                       dev_t *         p );

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


#endif //DEV_H
