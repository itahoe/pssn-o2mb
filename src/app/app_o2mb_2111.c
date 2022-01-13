/**
  * @file    app_o2mb_2111.c
  * @brief   Main Application
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <time.h>
#include "app.h"
#include "dev.h"
#include "mdbs.h"
#include "stm32.h"
//#include "sys.h"
#include "sens.h"
#include "ad7799.h"
#include "lps25.h"
#include "lmp91000.h"
#include "nvm.h"
//#include "filter_13.h"
//#include "filter_23.h"
//#include "filter_47.h"
#include "filter_025_125_60db_29taps.h"


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
static  Filter          filter;


const   dev_conf_t      dev_conf        =
{
        .device_id              = 0x02FF,
        .hardware_id            = 0x2011,
        .firmware_id            = { 0xAABB, 0xCCDD, },
        .serial_num             = { 0x0011, 0x2233, 0x4455, 0x6677, 0x8899, 0xAABB, 0xCCDD, 0xEEFF, },
        .adc_vref_mV            = 2500,
        .adc_resolution_bits    = 24,
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
app_nvm_restore( void )
{
        nvm_read16( NVM_ADDR_STARTS_COUNT,              &dev.mcu.starts_cnt,            1 );
        dev.mcu.starts_cnt++;
        nvm_write16( NVM_ADDR_STARTS_COUNT,             &dev.mcu.starts_cnt,            1 );

        nvm_read16( NVM_ADDR_AFE_BIAS,                  &dev.afe.bias,                  1 );

        nvm_read16( NVM_ADDR_AFE_ADC_REG_MODE,          &dev.ad7799.reg.mode.u16,       1 );
        nvm_read16( NVM_ADDR_AFE_ADC_REG_CONF,          &dev.ad7799.reg.conf.u16,       1 );

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
        dev.evt.mcu_ser1_recv   =   true;
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
app_1msec_tick_hook( void )
{
        if( ++( dev.mcu_systick_1hz ) >= 1000 )
        {
                dev.mcu_systick_1hz     = 0;
                dev.evt.mcu_systick_1hz = true;
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
        dev.evt.mcu_adc         = true;
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
HAL_ADC_ConvCpltCallback(                       ADC_HandleTypeDef *     hadc )
{
        dev.evt.mcu_adc         = true;
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

        //printf( "resp: %X\n", resp );

        dev.ad7799.reg.mode.u16         = ad7799_reg_mode_read();
        dev.ad7799.reg.mode.rate        = AD7799_RATE_4_17_Hz;
        dev.ad7799.reg.mode.mode        = AD7799_MODE_SINGLE;
        ad7799_reg_mode_write( dev.ad7799.reg.mode );
        ad7799_reg_conf_write( dev.ad7799.reg.conf );


        stm32_i2c2_init();

        lmp91000_write_unlock();
        //lmp91000_set_tia_gain(          LMP91000_TIA_GAIN_2K75          );
        //lmp91000_set_tia_gain(          LMP91000_TIA_GAIN_3K5           );
        //lmp91000_set_tia_gain(          LMP91000_TIA_GAIN_7K            );
        //lmp91000_set_tia_gain(          LMP91000_TIA_GAIN_14K           );
        //lmp91000_set_tia_gain(          LMP91000_TIA_GAIN_35K           );
        //lmp91000_set_tia_gain(          LMP91000_TIA_GAIN_120K          );
        lmp91000_set_tia_gain(          LMP91000_TIA_GAIN_350K          );

        //lmp91000_set_rload(             LMP91000_RLOAD_10R              );
        //lmp91000_set_rload(             LMP91000_RLOAD_33R              );
        lmp91000_set_rload(             LMP91000_RLOAD_50R              );
        //lmp91000_set_rload(             LMP91000_RLOAD_100R              );

        lmp91000_set_reference_source(  LMP91000_REF_SRC_EXTERNAL       );
        //lmp91000_set_internal_zero(     LMP91000_INT_ZERO_50_PERCENT    );
        lmp91000_set_internal_zero(     LMP91000_INT_ZERO_20_PERCENT    );
        lmp91000_set_fet_short(         LMP91000_FET_SHORT_DISABLE       );
        //lmp91000_set_mode(              LMP91000_MODE_STANDBY           );
        lmp91000_write_lock();
        lmp91000_set_mode(              LMP91000_MODE_3_LEAD_CELL       );


        lps25_set_ctl( LPS25_CTL_POWER_DOWN, 1 );
        lps25_set_data_rate( LPS25_RATE_1_Hz );

/*
        //stm32_tim3_config_period( 255 );
        stm32_tim3_config_period( 64 );
        stm32_tim3_init();
        stm32_tim3_start();
*/

/*
        stm32_tim6_config_period( 1 );
        stm32_tim6_init();
        stm32_tim6_start();
*/
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
                if( dev.evt.mcu_ser1_recv )
                {
                        dev.evt.mcu_ser1_recv   = false;

                        len     = MDBS_RTU_ADU_SIZEOF - stm32_usart1_dma_recv_get_ndtr();
                        //APP_TRACE("%d - ", len );
                        len     = mdbs_rtu_slave( CFG_MDBS_DEV_ADDR, modbus_adu, len );
                        //APP_TRACE("%d\n", len );

                        //for(int i=0; i<len;i++) APP_TRACE("%02X ", modbus_adu[i] ); APP_TRACE("\n");

                        if( len > 0 )
                        {
                                int i = stm32_usart1_xmit_dma( modbus_adu, len );

                                //APP_TRACE("stm32_usart1_xmit_dma() = %d\n", i );

                                stm32_led_sts_flash( UI_LED_FLSH_SHRT_TCKS );
                        }

                        stm32_usart1_recv_dma( modbus_adu, MDBS_RTU_ADU_SIZEOF );

                        //APP_TRACE( "ser1_recv done\n" );
               } //ser1_recv

                if( dev.evt.mcu_adc )
                {
                        dev.evt.mcu_adc         = false;
                        dev.mcu.vref_mV         = stm32_adc_raw_to_vref_mV( adc_raw[ 1] );
                        dev.mcu.degc            = stm32_adc_raw_to_celsius( adc_raw[ 0], dev.mcu.vref_mV );
                } //adc

                if( dev.evt.mcu_systick_1hz )
                {
                        dev.evt.mcu_systick_1hz = false;

                        sens.temp.raw           = lps25_get_temperature_raw();
                        sens.temp.digc.f32      = lps25_temperature_raw_to_digc( sens.temp.raw );

                        //sens.pres.raw.u32       = 0x3E8670;
                        sens.pres.raw.i32       = lps25_get_pressure_raw();
                        sens.pres.hPa.f32       = lps25_pressure_raw_to_hpa( sens.pres.raw.i32 );


                        lmp91000_set_mode( LMP91000_MODE_3_LEAD_CELL );
                        //lmp91000_set_mode( LMP91000_MODE_2_LEAD_CELL );
                        //lmp91000_set_mode( LMP91000_MODE_TEMP_MEAS_TIA_ON );

                        //HAL_Delay( 10 );

                        dev.afe.adc_raw.u32     = ad7799_read_single();
                        sens.oxgn.raw.i32       = sens_average( &sens.avrg, dev.afe.adc_raw.u32 );

                        //sens.oxgn.raw.u32       = sens_average( &sens.avrg, ad7799_read_single() );
                        //Filter_put( &filter, ad7799_read_single() );
                        //sens.oxgn.raw.u32       = Filter_get( &filter );;
                        sens.oxgn.ppm.f32       = sens_oxgn_raw_to_ppm( &sens );
                        sens.avrg.slope         = sens_oxgn_get_slope( &sens.avrg, sens.oxgn.raw.i32 );


                        /*
                        lmp91000_set_mode( LMP91000_MODE_TEMP_MEAS_TIA_ON );
                        HAL_Delay( 1 );
                        sens.temp.raw           = ad7799_read_single();
                        //float adc_lsb_mV      = ADC_VREF_mV / ((uint32_t) 1 << 24);
                        sens.temp.digc.f32      = lmp91000_temperature_mV_to_digc( sens.temp.raw * 0.0001490116119384765625);
                        //sens.temp.digc.f32      = lmp91000_temperature_mV_to_digc( sens.temp.raw * float adc_lsb_mV );

                        //lmp91000_set_mode( LMP91000_MODE_STANDBY );
                        */

                } //app.evt.sens
        }
}
