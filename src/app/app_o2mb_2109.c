/**
  * @file    app.c
  * @brief   Main Application
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <time.h>
#include "app.h"
#include "mdbs.h"
#include "stm32.h"
#include "sys.h"
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
#define AVERAGE_BUF_SIZEOF              2
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

        .mcu.serial_num         = sys_serial_num,
        .mcu.device_id          = sys_device_id,
        //.mcu.hardware_id        = sys_hardware_id,
        .mcu.firmware_id        = sys_firmware_id,
};

        uint16_t                VirtAddVarTab[ NVM_ADDR_MAX ]  =
{
        NVM_ADDR_STARTS_COUNT,
        NVM_ADDR_SENS_OFST_RAW,
        NVM_ADDR_02,
        NVM_ADDR_03,
        NVM_ADDR_04,
        NVM_ADDR_05,
        NVM_ADDR_06,
        NVM_ADDR_07,

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
app_nvm_restore()
{
        nvm_read16( NVM_ADDR_STARTS_COUNT,              &sens.mcu.starts_cnt,                   1 );
        sens.mcu.starts_cnt++;
        nvm_write16( NVM_ADDR_STARTS_COUNT,             &sens.mcu.starts_cnt,                   1 );

        nvm_read16( NVM_ADDR_SENS_OFST_RAW,             &sens.oxgn.offset,                      1 );

        nvm_read16( NVM_ADDR_TRIM_P0_TIMESTMP_HI,  &sens.trim.point[ 0].timestmp.u16[ 1],     1 );
        nvm_read16( NVM_ADDR_TRIM_P0_TIMESTMP_LO,  &sens.trim.point[ 0].timestmp.u16[ 0],     1 );
        nvm_read16( NVM_ADDR_TRIM_P0_OXGN_PPM_HI,  &sens.trim.point[ 0].oxgn_ppm.u16[ 1],     1 );
        nvm_read16( NVM_ADDR_TRIM_P0_OXGN_PPM_LO,  &sens.trim.point[ 0].oxgn_ppm.u16[ 0],     1 );
        nvm_read16( NVM_ADDR_TRIM_P0_OXGN_RAW_HI,  &sens.trim.point[ 0].oxgn_raw.u16[ 1],     1 );
        nvm_read16( NVM_ADDR_TRIM_P0_OXGN_RAW_LO,  &sens.trim.point[ 0].oxgn_raw.u16[ 0],     1 );

        //nvm_read16( NVM_ADDR_TRIM_P0_TEMP_RAW_HI,  &sens.trim.temp_raw[ 0].u16[ 1],        1 );
        //nvm_read16( NVM_ADDR_TRIM_P0_TEMP_RAW_LO,  &sens.trim.temp_raw[ 0].u16[ 0],        1 );
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
app_sens_hook( void )
{
        //app.evt.sens    =   true;
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


        ad7799_init();
        ad7799_reset();
        ad7799_set_mode(        AD7799_MODE_IDLE                );
        ad7799_set_rate(        AD7799_RATE_4_17_Hz             );
        ad7799_set_burnout(     false                           );
        ad7799_set_unipolar(    true                            );
        ad7799_set_buffered(    false                            );
        ad7799_set_gain(        AD7799_GAIN_1                   );
        ad7799_set_refdet(      false                           );
        ad7799_set_pwr_swtch(   false                           );

        ad7799_set_channel(     AD7799_CHNL_AIN1P_AIN1M         );
        //ad7799_set_channel(     AD7799_CHNL_AVDD_MONITOR        );

        //ad7799_set_mode( AD7799_MODE_CAL_INT_ZERO );
        //while( !ad7799_sts_ready() );
        //ad7799_set_mode( AD7799_MODE_CAL_INT_FULL );
        //while( !ad7799_sts_ready() );

        stm32_i2c2_init();

        lmp91000_write_unlock();
        lmp91000_set_tia_gain(          LMP91000_TIA_GAIN_2K75          );
        lmp91000_set_rload(             LMP91000_RLOAD_10R              );
        lmp91000_set_reference_source(  LMP91000_REF_SRC_EXTERNAL       );
        lmp91000_set_internal_zero(     LMP91000_INT_ZERO_20_PERCENT    );
        lmp91000_set_fet_short(         LMP91000_FET_SHORT_DISABLE      );
        lmp91000_set_mode(              LMP91000_MODE_3_LEAD_CELL       );
        lmp91000_write_lock();

        lps25_set_ctl( LPS25_CTL_POWER_DOWN, 1 );
        lps25_set_data_rate( LPS25_RATE_1_Hz );


        stm32_tim3_config_period( 1 );
        stm32_tim3_init();
        stm32_tim3_start();

        stm32_adc_init();
        stm32_adc_start( adc_raw, ADC_RAW_SIZEOF );

        stm32_usart1_config_baudrate( CFG_MDBS_BAUDRATE );
        stm32_usart1_init();
        stm32_usart1_recv_dma( modbus_adu, MDBS_RTU_ADU_SIZEOF );


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
                        sens.mcu.vref_mV        = stm32_adc_raw_to_vref_mV( adc_raw[ 1] );
                        sens.mcu.degc           = stm32_adc_raw_to_celsius( adc_raw[ 0], sens.mcu.vref_mV );
                } //adc

                if( app.evt.tick_1hz )
                {
                        app.evt.tick_1hz        = false;

                        sens.temp.raw           = lps25_get_temperature_raw();
                        sens.temp.digc.f32      = lps25_temperature_raw_to_digc( sens.temp.raw );

                        //sens.pres.raw.u32       = 0x3E8670;
                        sens.pres.raw.i32       = lps25_get_pressure_raw();
                        sens.pres.hPa.f32       = lps25_pressure_raw_to_hpa( sens.pres.raw.i32 );

                        sens.oxgn.raw.u32       = sens_average( &sens.avrg, ad7799_read_single() );
                        //Filter_put( &filter, ad7799_read_single() );
                        //sens.oxgn.raw.u32       = Filter_get( &filter );;
                        sens.oxgn.ppm.f32       = sens_oxgn_raw_to_ppm( &sens );
                        //sens.avrg.slope         = sens_oxgn_get_slope( &sens.avrg, sens.oxgn.raw.i32 );
                } //app.evt.sens
        }
}
