/**
  * @file    app_adc_2003.c
  * @brief   Main Application
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <time.h>

#include "mdbs.h"
#include "stm32.h"
#include "sys.h"
#include "sens.h"
#include "lps25.h"
#include "nvm.h"
#include "lm75.h"
#include "app.h"


const   uint32_t        device_id       = 0xAABBCCEE;

/*******************************************************************************
* PRIVATE TYPES
*******************************************************************************/
typedef struct  app_evt_s
{
        bool                    mdbs_recv;
        //bool                    lm75;
        bool                    tick_1hz;
        bool                    adc;
        bool                    sens;
} app_evt_t;


typedef struct  app_s
{
        app_evt_t               evt;
        size_t                  tick_1sec;
} app_t;


//float   sens_get_k_temp_digc( const float );

#define OXGN_RAW_SIZEOF                 3
#define AVERAGE_BUF_SIZEOF              8


/*******************************************************************************
* PRIVATE VARIABLES
*******************************************************************************/
static  uint8_t         modbus_adu[ MDBS_RTU_ADU_SIZEOF ];
static  uint16_t        adc_raw[ OXGN_RAW_SIZEOF ];
static  uint32_t        oxgn_avrg_buf[ AVERAGE_BUF_SIZEOF ];
static  uint32_t        pres_avrg_buf[ AVERAGE_BUF_SIZEOF ];

static  app_t           app;



        sens_t          sens    =
{
        .avrg.buf               = oxgn_avrg_buf,
        .avrg.buf_sizeof        = AVERAGE_BUF_SIZEOF,
        .avrg.idx               = 0,
        .avrg.sum               = 0,

        //.temp.avrg.buf          = temp_avrg_buf,
        //.temp.avrg.buf_sizeof   = AVERAGE_BUF_SIZEOF,
        //.temp.avrg.idx          = 0,
        //.temp.avrg.sum          = 0,

        .pres.avrg.buf          = pres_avrg_buf,
        .pres.avrg.buf_sizeof   = AVERAGE_BUF_SIZEOF,
        .pres.avrg.idx          = 0,
        .pres.avrg.sum          = 0,

        .mcu.serial_num         = sys_serial_num,
        //.mcu.device_id          = sys_device_id,
        .mcu.hardware_id        = sys_hardware_id,
        .mcu.firmware_id        = sys_firmware_id,
};

        uint16_t                VirtAddVarTab[ NVM_ADDR_MAX ]  =
{
        NVM_ADDR_STARTS_COUNT,
        NVM_ADDR_SENS_OFST_RAW,
        //NVM_ADDR_02,
        //NVM_ADDR_03,
        NVM_ADDR_AFE_GAIN,
        NVM_ADDR_AFE_CHANNEL,
        NVM_ADDR_04,
        NVM_ADDR_05,
        //NVM_ADDR_06,
        //NVM_ADDR_07,
        NVM_ADDR_AFE_K_TEMP_DRIFT_HI,
        NVM_ADDR_AFE_K_TEMP_DRIFT_LO,

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
*
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
void
app_mdbs_recv_hook( void )
{
        app.evt.mdbs_recv       =   true;
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
                //app.evt.lm75    = true;
                app.evt.tick_1hz        = true;
                app.evt.sens            =   true;
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
        //nvm_read16( NVM_ADDR_TRIM_P0_TEMP_RAW_HI,  (uint16_t *) &sens.trim.point[ 0].temp_raw,             1 );
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
        //nvm_read16( NVM_ADDR_TRIM_P1_TEMP_RAW_HI,  (uint16_t *) &sens.trim.point[ 1].temp_raw,             1 );

        nvm_read16( NVM_ADDR_AFE_K_TEMP_DRIFT_HI,  &sens.afe.k_temp_drift.u16[ 1],     1 );
        nvm_read16( NVM_ADDR_AFE_K_TEMP_DRIFT_LO,  &sens.afe.k_temp_drift.u16[ 0],     1 );

        nvm_read16( NVM_ADDR_TRIM_P1_TEMP_DIGC_HI,  &sens.trim.point[ 1].temp_digc.u16[ 1], 1 );
        nvm_read16( NVM_ADDR_TRIM_P1_TEMP_DIGC_LO,  &sens.trim.point[ 1].temp_digc.u16[ 0], 1 );
        nvm_read16( NVM_ADDR_TRIM_P1_PRES_RAW_HI,  &sens.trim.point[ 1].pres_raw.u16[ 1],     1 );
        nvm_read16( NVM_ADDR_TRIM_P1_PRES_RAW_LO,  &sens.trim.point[ 1].pres_raw.u16[ 0],     1 );
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
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
app_set_afe_gain(                       const   uint16_t        gain )
{
        return( 0 );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
app_set_afe_channel(                    const   uint16_t        channel )
{
        return( 0 );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
app_set_afe_offset(                     const   uint16_t        offset )
{
        return( 0 );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
app_set_afe_control(                    const   uint16_t        offset )
{
        return( 0 );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int
app_set_afe_unipolar(                   const   int             bval )
{
        return( 0 );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int
app_set_afe_input_short(                const   int             bval )
{
        return( 0 );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int
app_set_afe_buffer_enable(              const   int             bval )
{
        return( 0 );
}



/**
  * @brief  The application entry point.
  * @retval int
  */
int main( void )
{
        size_t          len;


        HAL_Init();

        stm32_clock_init();

        stm32_led_sts_init();
        stm32_led_sts_set( true );

        nvm_init();
        app_nvm_restore();
        sens_trim_restore( &sens.trim );


        stm32_usart1_config_baudrate( CFG_MDBS_BAUDRATE );
        stm32_usart1_init();
        stm32_usart1_recv_dma( modbus_adu, MDBS_RTU_ADU_SIZEOF );

        stm32_adc_config_sps( 1 );
        stm32_adc_init();
        stm32_adc_start( adc_raw, OXGN_RAW_SIZEOF );

        sens.temp.raw           = -8400;
        sens.temp.digc.f32      = 25.0f;
        sens.pres.raw.u32       = 0x800000;
        sens.pres.hPa.f32       = 1000.0f;

        while( true )
        {
                if( app.evt.mdbs_recv )
                {
                        app.evt.mdbs_recv       = false;

                        len     = MDBS_RTU_ADU_SIZEOF - stm32_usart1_dma_recv_get_ndtr();
                        len     = mdbs_rtu_slave( CFG_MDBS_DEV_ADDR, modbus_adu, len );

                        if( len > 0 )
                        {
                                stm32_usart1_xmit_dma( modbus_adu, len );
                                stm32_led_sts_flash( UI_LED_FLSH_SHRT_TCKS );
                        }

                        stm32_usart1_recv_dma( modbus_adu, MDBS_RTU_ADU_SIZEOF );

                        //APP_TRACE( "RAW: %08X\tPPM: %d\tTG: %f\tOFST: %f\n", sens.oxgn.raw.u32, sens.oxgn.ppm.u32, sens.trim.tg, sens.trim.ofst );
                        //APP_TRACE( "RAW: %08X\tPPM: %d\tkT: %f\n", sens.oxgn.raw.u32, sens.oxgn.ppm.u32, sens_get_k_temp_digc( sens.temp.digc.f32 ) );
                        //APP_TRACE( "RAW: %08X\tPPM: %d\tt: %f\n", sens.oxgn.raw.u32, sens.oxgn.ppm.u32, sens.temp.digc.f32 );

               } //ser1_recv

                if( app.evt.adc )
                {
                        app.evt.adc             = false;

                        sens.oxgn.raw.u32       = sens_average( &sens.avrg, adc_raw[ 0] );
                        sens.oxgn.ppm.f32       = sens_oxgn_raw_to_ppm( &sens );

                        sens.mcu.degc           = stm32_adc_raw_to_celsius( adc_raw[ 1], sens.mcu.vref_mV );
                        sens.mcu.vref_mV        = stm32_adc_raw_to_vref_mV( adc_raw[ 2] );
                } //adc

                if( app.evt.tick_1hz )
                {
                        app.evt.tick_1hz        = false;
                }

                if( app.evt.sens )
                {
                        app.evt.sens            =   false;

                        //sens.pres.raw.u32       = lps25_get_pressure_raw();
                        //sens.pres.hPa.f32       = lps25_pressure_raw_to_hpa( sens.pres.raw.i32 );
                        //sens.temp.raw           = lps25_get_temperature_raw();
                        //sens.temp.digc.f32      = lps25_temperature_raw_to_digc( sens.temp.raw );
                        //sens.oxgn.raw.u32       = sens_oxgn_raw_avrg( &sens.avrg, ad7799_read_single() );
                        //sens.oxgn.ppm.i32       = sens_oxgn_raw_to_ppm( &sens );
                        //sens.oxgn.ppm.f32       = sens_oxgn_raw_to_ppm( &sens );
                        //sens.avrg.slope         = sens_oxgn_get_slope( &sens.avrg, sens.oxgn.raw.i32 );

                } //app.evt.sens

        }
}
