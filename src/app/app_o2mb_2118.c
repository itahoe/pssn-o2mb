/**
  * @file    app_o2mb_2118.c
  * @brief   Main Application
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <time.h>
#include "app.h"
#include "dev.h"
#include "mdbs.h"
#include "stm32.h"
#include "sens.h"
#include "lps25.h"
#include "nvm.h"
//#include "filter_13.h"
#include "filter_23.h"
//#include "filter_47.h"
//#include "filter_025_125_60db_29taps.h"
#include "config.h"


/*******************************************************************************
* PRIVATE TYPES
*******************************************************************************/
typedef struct  app_evt_s
{
        bool                    ser1_recv;
        bool                    tick_1hz;
        bool                    adc;
} app_evt_t;


typedef struct  app_s
{
        app_evt_t               evt;
        size_t                  tick_1sec;
} app_t;


/*******************************************************************************
* PRIVATE DEFINES
*******************************************************************************/
//#define AVERAGE_BUF_SIZEOF              128
#define AVERAGE_BUF_SIZEOF              8
#define ADC_RAW_SIZEOF                  2


/*******************************************************************************
* LOCAL VARIABLES
*******************************************************************************/
static  uint8_t         modbus_adu[ MDBS_RTU_ADU_SIZEOF ];
static  uint16_t        adc_raw[ ADC_RAW_SIZEOF ];
static  uint32_t        oxgn_avrg_buf[ AVERAGE_BUF_SIZEOF ];
static  uint32_t        pres_avrg_buf[ AVERAGE_BUF_SIZEOF ];
static  app_t           app;
static  Filter          filter;

const   dev_conf_t      dev_conf        =
{
        .device_id              = 0x02FF,
        .hardware_id            = 0x2118,
        .firmware_id            = { 0x2112, 0x1300, },
        .serial_num             = { 0x0011, 0x2233, 0x4455, 0x6677, 0x8899, 0xAABB, 0xCCDD, 0xEEFF, },
        .adc_vref_mV            = 2500,
        //.adc_resolution_bits    = 24,
        .adc_resolution_bits    = 32,
};

        dev_t           dev             =
{
        .conf           = &dev_conf,
};


/*******************************************************************************
* GLOBAL VARIABLES
*******************************************************************************/
        sens_t          sens    =
{
        .avrg.buf               = oxgn_avrg_buf,
        .avrg.buf_sizeof        = AVERAGE_BUF_SIZEOF,
        .avrg.idx               = 0,
        .avrg.sum               = 0,

        .pres.avrg.buf          = pres_avrg_buf,
        .pres.avrg.buf_sizeof   = AVERAGE_BUF_SIZEOF,
        .pres.avrg.idx          = 0,
        .pres.avrg.sum          = 0,
};

        uint16_t                VirtAddVarTab[ NVM_ADDR_MAX ]  =
{
        NVM_ADDR_STARTS_COUNT,
        NVM_ADDR_AFE_BIAS,
        NVM_ADDR_AFE_ADC_REG_MODE,
        NVM_ADDR_AFE_ADC_REG_CONF,
        NVM_ADDR_AFE_K_TEMP_DRIFT_HI,
        NVM_ADDR_AFE_K_TEMP_DRIFT_LO,
        NVM_ADDR_AFE_K_PRES_DRIFT_HI,
        NVM_ADDR_AFE_K_PRES_DRIFT_LO,

        NVM_ADDR_TRIM_P0_TIMESTMP_LO,
        NVM_ADDR_TRIM_P0_TIMESTMP_HI,
        NVM_ADDR_TRIM_P0_OXGN_PPM_LO,
        NVM_ADDR_TRIM_P0_OXGN_PPM_HI,
        NVM_ADDR_TRIM_P0_OXGN_RAW_LO,
        NVM_ADDR_TRIM_P0_OXGN_RAW_HI,
        NVM_ADDR_TRIM_P0_TEMP_DIGC_LO,
        NVM_ADDR_TRIM_P0_TEMP_DIGC_HI,
        NVM_ADDR_TRIM_P0_PRES_RAW_LO,
        NVM_ADDR_TRIM_P0_PRES_RAW_HI,
        NVM_ADDR_18,
        NVM_ADDR_19,

        NVM_ADDR_TRIM_P1_TIMESTMP_LO,
        NVM_ADDR_TRIM_P1_TIMESTMP_HI,
        NVM_ADDR_TRIM_P1_OXGN_PPM_LO,
        NVM_ADDR_TRIM_P1_OXGN_PPM_HI,
        NVM_ADDR_TRIM_P1_OXGN_RAW_LO,
        NVM_ADDR_TRIM_P1_OXGN_RAW_HI,
        NVM_ADDR_TRIM_P1_TEMP_DIGC_LO,
        NVM_ADDR_TRIM_P1_TEMP_DIGC_HI,
        NVM_ADDR_TRIM_P1_PRES_RAW_LO,
        NVM_ADDR_TRIM_P1_PRES_RAW_HI,
        NVM_ADDR_30,
        NVM_ADDR_31,
};


//afe_t           afe;
dev_t           dev;


/*******************************************************************************
* PRIVATE FUNCTIONS
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
static
void
app_nvm_restore()
{
        nvm_read16( NVM_ADDR_STARTS_COUNT,          &dev.starts_cnt,                   1 );
        dev.starts_cnt++;
        nvm_write16( NVM_ADDR_STARTS_COUNT,         &dev.starts_cnt,                   1 );

        nvm_read16( NVM_ADDR_AFE_BIAS,              &dev.afe.bias,                      1 );
        nvm_read16( NVM_ADDR_AFE_ADC_REG_MODE,      &dev.ad7799.reg.mode.u16,       1 );
        nvm_read16( NVM_ADDR_AFE_ADC_REG_CONF,      &dev.ad7799.reg.conf.u16,       1 );

        nvm_read16( NVM_ADDR_AFE_K_TEMP_DRIFT_HI,  &sens.drift_k_temp.u16[ 1],     1 );
        nvm_read16( NVM_ADDR_AFE_K_TEMP_DRIFT_LO,  &sens.drift_k_temp.u16[ 0],     1 );
        nvm_read16( NVM_ADDR_AFE_K_PRES_DRIFT_HI,  &sens.drift_k_pres.u16[ 1],     1 );
        nvm_read16( NVM_ADDR_AFE_K_PRES_DRIFT_LO,  &sens.drift_k_pres.u16[ 0],     1 );

        nvm_read16( NVM_ADDR_TRIM_P0_TIMESTMP_HI,  &sens.trim.point[ 0].timestmp.u16[ 1],     1 );
        nvm_read16( NVM_ADDR_TRIM_P0_TIMESTMP_LO,  &sens.trim.point[ 0].timestmp.u16[ 0],     1 );
        nvm_read16( NVM_ADDR_TRIM_P0_OXGN_PPM_HI,  &sens.trim.point[ 0].oxgn_ppm.u16[ 1],     1 );
        nvm_read16( NVM_ADDR_TRIM_P0_OXGN_PPM_LO,  &sens.trim.point[ 0].oxgn_ppm.u16[ 0],     1 );
        nvm_read16( NVM_ADDR_TRIM_P0_OXGN_RAW_HI,  &sens.trim.point[ 0].oxgn_raw.u16[ 1],     1 );
        nvm_read16( NVM_ADDR_TRIM_P0_OXGN_RAW_LO,  &sens.trim.point[ 0].oxgn_raw.u16[ 0],     1 );

        nvm_read16( NVM_ADDR_TRIM_P0_TEMP_DIGC_HI, &sens.trim.point[ 0].temp_digc.u16[ 1], 1 );
        nvm_read16( NVM_ADDR_TRIM_P0_TEMP_DIGC_LO, &sens.trim.point[ 0].temp_digc.u16[ 0], 1 );
        nvm_read16( NVM_ADDR_TRIM_P0_PRES_RAW_HI,  &sens.trim.point[ 0].pres_raw.u16[ 1],     1 );
        nvm_read16( NVM_ADDR_TRIM_P0_PRES_RAW_LO,  &sens.trim.point[ 0].pres_raw.u16[ 0],     1 );

        nvm_read16( NVM_ADDR_TRIM_P1_TIMESTMP_HI,  &sens.trim.point[ 1].timestmp.u16[ 1],     1 );
        nvm_read16( NVM_ADDR_TRIM_P1_TIMESTMP_LO,  &sens.trim.point[ 1].timestmp.u16[ 0],     1 );
        nvm_read16( NVM_ADDR_TRIM_P1_OXGN_PPM_HI,  &sens.trim.point[ 1].oxgn_ppm.u16[ 1],     1 );
        nvm_read16( NVM_ADDR_TRIM_P1_OXGN_PPM_LO,  &sens.trim.point[ 1].oxgn_ppm.u16[ 0],     1 );
        nvm_read16( NVM_ADDR_TRIM_P1_OXGN_RAW_HI,  &sens.trim.point[ 1].oxgn_raw.u16[ 1],     1 );
        nvm_read16( NVM_ADDR_TRIM_P1_OXGN_RAW_LO,  &sens.trim.point[ 1].oxgn_raw.u16[ 0],     1 );

        //nvm_read16( NVM_ADDR_TRIM_P1_TEMP_RAW_HI,  &sens.trim.temp_raw[ 1].u16[ 1],     1 );
        //nvm_read16( NVM_ADDR_TRIM_P1_TEMP_RAW_LO,  &sens.trim.temp_raw[ 1].u16[ 0],     1 );
        nvm_read16( NVM_ADDR_TRIM_P1_TEMP_DIGC_HI, &sens.trim.point[ 1].temp_digc.u16[ 1], 1 );
        nvm_read16( NVM_ADDR_TRIM_P1_TEMP_DIGC_LO, &sens.trim.point[ 1].temp_digc.u16[ 0], 1 );
        nvm_read16( NVM_ADDR_TRIM_P1_PRES_RAW_HI,  &sens.trim.point[ 1].pres_raw.u16[ 1],     1 );
        nvm_read16( NVM_ADDR_TRIM_P1_PRES_RAW_LO,  &sens.trim.point[ 1].pres_raw.u16[ 0],     1 );
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
app_ser1_recv_hook( void )
{
        app.evt.ser1_recv       =   true;
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
app_1msec_tick_hook( void )
{
        if( ++( app.tick_1sec ) >= 1000 )
        {
                app.tick_1sec           = 0;
                app.evt.tick_1hz        = true;
                //app.evt.lm75    = true;
                //app.evt.sens            = true;
        }
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
app_adc_eos_hook( void )
{
        app.evt.adc     =   true;
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
HAL_ADC_ConvCpltCallback(                       ADC_HandleTypeDef *     hadc )
{
        app.evt.adc     = true;
}



/*******************************************************************************
* MAIN
*******************************************************************************/
/**
  * @brief  The application entry point.
  * @retval int
  */
int
main( void )
{
        size_t          len;


        HAL_Init();

        stm32_clock_init();

        stm32_led_sts_init();
        stm32_led_sts_set( true );

        nvm_init();
        app_nvm_restore();
        sens_trim_restore( &sens.trim );
        Filter_init( &filter );


        stm32_spi1_init();

        int resp        = ad7799_init();

        //dev.ad7799.reg.mode.u16         = ad7799_reg_mode_read();
        dev.ad7799.reg.mode.u16         = ad7799_read( AD7799_REG_MODE );
        dev.ad7799.reg.mode.rate        = AD7799_RATE_4_17_Hz;
        dev.ad7799.reg.mode.mode        = AD7799_MODE_SINGLE;
        //ad7799_reg_mode_write( dev.ad7799.reg.mode );
        //ad7799_reg_write( AD7799_REG_MODE, dev.ad7799.reg.mode );
        ad7799_write( AD7799_REG_MODE, dev.ad7799.reg.mode.u16 );
        //ad7799_reg_conf_write( dev.ad7799.reg.conf );
        ad7799_write( AD7799_REG_CONF, dev.ad7799.reg.conf.u16 );


        stm32_i2c1_init();

        lps25_set_ctl( LPS25_CTL_POWER_DOWN, 1 );
        lps25_set_data_rate( LPS25_RATE_1_Hz );


        stm32_tim3_config_period( 64 );
        stm32_tim3_init();
        stm32_tim3_start();
        stm32_tim3_ch4_update( dev.afe.bias );


        stm32_adc_init();
        stm32_adc_start( adc_raw, ADC_RAW_SIZEOF );

        stm32_usart1_config_baudrate( CFG_MDBS_BAUDRATE );
        stm32_usart1_init();
        stm32_usart1_recv_dma( modbus_adu, MDBS_RTU_ADU_SIZEOF );

        sens.temp.raw           = 0x5555;
        sens.temp.digc.f32      = 25.0;
        sens.pres.raw.u32       = 0x3E8670;
        sens.pres.hPa.f32       = lps25_pressure_raw_to_hpa( sens.pres.raw.i32 );


        while( true )
        {
                if( app.evt.ser1_recv )
                {
                        app.evt.ser1_recv       = false;

                        len     = MDBS_RTU_ADU_SIZEOF - stm32_usart1_dma_recv_get_ndtr();
                        len     = mdbs_rtu_slave( CFG_MDBS_DEV_ADDR, modbus_adu, len );

                        if( len > 0 )
                        {
                                stm32_usart1_xmit_dma( modbus_adu, len );
                                stm32_led_sts_flash( UI_LED_FLSH_SHRT_TCKS );
                        }

                        stm32_usart1_recv_dma( modbus_adu, MDBS_RTU_ADU_SIZEOF );
               } //ser1_recv

                if( app.evt.adc )
                {
                        app.evt.adc             = false;
                        dev.mcu.vref_mV         = stm32_adc_raw_to_vref_mV( adc_raw[ 1] );
                        dev.mcu.degc            = stm32_adc_raw_to_celsius( adc_raw[ 0], dev.mcu.vref_mV );
                } //adc

                if( app.evt.tick_1hz )
                {
                        app.evt.tick_1hz        = false;


                        /*
                        dev_sens_temp_update( &dev );
                        dev_sens_pres_update( &dev );
                        dev_sens_cell_update( &dev );
                        */

                        sens.temp.raw           = lps25_get_temperature_raw();
                        sens.temp.digc.f32      = lps25_temperature_raw_to_digc( sens.temp.raw );

                        sens.pres.raw.i32       = lps25_get_pressure_raw();
                        sens.pres.hPa.f32       = lps25_pressure_raw_to_hpa( sens.pres.raw.i32 );

                        //sens.oxgn.raw.i32       = sens_average( &sens.avrg, ad7799_get_sample() );

                        //dev.meas.sens.raw.i32   = ad7799_get_sample();

                        dev_proc( &dev );

                        sens.oxgn.raw.i32       = sens_average( &sens.avrg, dev.meas.sens.raw.i32 );
                        Filter_put( &filter, sens.oxgn.raw.i32 );
                        sens.oxgn.raw.i32       = Filter_get( &filter );

                        sens.oxgn.ppm.f32       = sens_oxgn_raw_to_ppm( &sens );
                        //sens.avrg.slope         = sens_oxgn_get_slope( &sens.avrg, sens.oxgn.raw.i32 );
                        //sens.avrg.slope         = sens_oxgn_get_slope( &sens.avrg, dev.afe.adc_raw.u32 );
                } //app.evt.sens
        }
}
