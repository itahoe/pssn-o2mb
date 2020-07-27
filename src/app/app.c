/**
  * @file    app.c
  * @brief   Main Application
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <time.h>

#include "mdbs.h"
#include "ui_led.h"
#include "sys.h"
#include "sens.h"
#include "lm75.h"
#include "app.h"


#define OXGN_RAW_SIZEOF                 2
#define SENS_O2_AVERAGE_BUF_SIZEOF      16


static  uint8_t         modbus_adu[ MDBS_RTU_ADU_SIZEOF ];
static  uint16_t        adc_raw[ OXGN_RAW_SIZEOF ];
static  int32_t         avrg_buf[ SENS_O2_AVERAGE_BUF_SIZEOF ];
        app_t           app;

        sens_t          sens    =
{
        .avrg.buf               = avrg_buf,
        .avrg.buf_sizeof        = SENS_O2_AVERAGE_BUF_SIZEOF,
        .avrg.idx               = 0,
        .avrg.sum               = 0,
        .mcu.serial_num         = sys_serial_num,
        .mcu.device_id          = sys_device_id,
        .mcu.hardware_id        = sys_hardware_id,
        .mcu.firmware_id        = sys_firmware_id,
};

        uint16_t                VirtAddVarTab[ SYS_NVM_ADDR_MAX ]  =
{
        SYS_NVM_ADDR_STARTS_COUNT,
        SYS_NVM_ADDR_SENS_OFST_RAW,
        SYS_NVM_ADDR_02,
        SYS_NVM_ADDR_03,
        SYS_NVM_ADDR_SENS_TRIM_0_PPM_LO,
        SYS_NVM_ADDR_SENS_TRIM_0_PPM_HI,
        SYS_NVM_ADDR_SENS_TRIM_0_RAW_LO,
        SYS_NVM_ADDR_SENS_TRIM_0_RAW_HI,
        SYS_NVM_ADDR_SENS_TRIM_0_TIMESTMP_LO,
        SYS_NVM_ADDR_SENS_TRIM_0_TIMESTMP_HI,
        SYS_NVM_ADDR_SENS_TRIM_1_PPM_LO,
        SYS_NVM_ADDR_SENS_TRIM_1_PPM_HI,
        SYS_NVM_ADDR_SENS_TRIM_1_RAW_LO,
        SYS_NVM_ADDR_SENS_TRIM_1_RAW_HI,
        SYS_NVM_ADDR_SENS_TRIM_1_TIMESTMP_LO,
        SYS_NVM_ADDR_SENS_TRIM_1_TIMESTMP_HI,
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
                app.tick_1sec   = 0;
                app.evt.lm75    = true;
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
        app.evt.sens    =   true;
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
        sys_nvm_read16( SYS_NVM_ADDR_STARTS_COUNT,              &sens.mcu.starts_cnt,                   1 );
        sens.mcu.starts_cnt++;
        sys_nvm_write16( SYS_NVM_ADDR_STARTS_COUNT,             &sens.mcu.starts_cnt,                   1 );

        sys_nvm_read16( SYS_NVM_ADDR_SENS_OFST_RAW,             &sens.oxgn.offset,                      1 );

        sys_nvm_read16( SYS_NVM_ADDR_SENS_TRIM_0_PPM_HI,        &sens.trim.ppm[ 0].u16[ 1],             1 );
        sys_nvm_read16( SYS_NVM_ADDR_SENS_TRIM_0_PPM_LO,        &sens.trim.ppm[ 0].u16[ 0],             1 );
        sys_nvm_read16( SYS_NVM_ADDR_SENS_TRIM_0_RAW_HI,        &sens.trim.raw[ 0].u16[ 1],             1 );
        sys_nvm_read16( SYS_NVM_ADDR_SENS_TRIM_0_RAW_LO,        &sens.trim.raw[ 0].u16[ 0],             1 );
        sys_nvm_read16( SYS_NVM_ADDR_SENS_TRIM_0_TIMESTMP_HI,   &sens.trim.timestamp[ 0].u16[ 1],       1 );
        sys_nvm_read16( SYS_NVM_ADDR_SENS_TRIM_0_TIMESTMP_LO,   &sens.trim.timestamp[ 0].u16[ 0],       1 );
        sys_nvm_read16( SYS_NVM_ADDR_SENS_TRIM_1_PPM_HI,        &sens.trim.ppm[ 1].u16[ 1],             1 );
        sys_nvm_read16( SYS_NVM_ADDR_SENS_TRIM_1_PPM_LO,        &sens.trim.ppm[ 1].u16[ 0],             1 );
        sys_nvm_read16( SYS_NVM_ADDR_SENS_TRIM_1_RAW_HI,        &sens.trim.raw[ 1].u16[ 1],             1 );
        sys_nvm_read16( SYS_NVM_ADDR_SENS_TRIM_1_RAW_LO,        &sens.trim.raw[ 1].u16[ 0],             1 );
        sys_nvm_read16( SYS_NVM_ADDR_SENS_TRIM_1_TIMESTMP_HI,   &sens.trim.timestamp[ 1].u16[ 1],       1 );
        sys_nvm_read16( SYS_NVM_ADDR_SENS_TRIM_1_TIMESTMP_LO,   &sens.trim.timestamp[ 1].u16[ 0],       1 );
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
int main( void )
{
        size_t          len;
        int32_t         sens_raw;


        HAL_Init();

        sys_clock_config();

        sys_nvm_init();
        app_nvm_restore();

        sens_trim_restore( &sens.trim );

        //sens_oxgn_ofst_init( 1 );
        //sens_oxgn_ofst_run();
        //sens_oxgn_ofst_set( sens.oxgn.offset );

        lm75_init();

        ui_led_sts_init();
        ui_led_sts_set( true );

        //sens_oxgn_init( CFG_SENS_OXGN_SMPLRATE_SPS );
        sens_oxgn_init( 1 );
        sens_oxgn_run( adc_raw, OXGN_RAW_SIZEOF );

        sys_ser1_init( CFG_MDBS_BAUDRATE );
        sys_ser1_recv( modbus_adu, MDBS_RTU_ADU_SIZEOF );


        while( true )
        {
                if( app.evt.ser1_recv )
                {
                        app.evt.ser1_recv       = false;

                        len     = MDBS_RTU_ADU_SIZEOF - bsp_ser1_dma_recv_get_ndtr();
                        len     = mdbs_rtu_slave( CFG_MDBS_DEV_ADDR, modbus_adu, len );

                        if( len > 0 )
                        {
                                sys_ser1_xmit( modbus_adu, len );
                                ui_led_sts_flash( UI_LED_FLSH_SHRT_TCKS );
                        }

                        sys_ser1_recv( modbus_adu, MDBS_RTU_ADU_SIZEOF );
                } //ser1_recv

                if( app.evt.lm75 )
                {
                        app.evt.lm75            = false;
                        sens.lm75.celsius       = lm75_read( LM75_REG_TEMP );
                } //lm75

                if( app.evt.adc )
                {
                        app.evt.adc             = false;
                        sens.mcu.vref_mV        = sens_mcu_raw_to_vref_mV( adc_raw[ 1] );
                        sens.mcu.celsius        = sens_mcu_raw_to_celsius( adc_raw[ 0], sens.mcu.vref_mV );
                } //adc

                if( app.evt.sens )
                {
                        app.evt.sens            =   false;

                        //sens_raw                = sens_oxgn_read() >> 8;
                        sens_raw                = sens_oxgn_read();
                        sens.oxgn.avrg          = sens_oxgn_raw_avrg( &sens.avrg, sens_raw );
                        sens.oxgn.ppm.i32       = sens_oxgn_avrg_to_ppm(  &sens.trim, sens.avrg.sum, sens.avrg.buf_sizeof, sens.lm75.celsius );
                        sens.oxgn.instability   = sens_oxgn_get_instability( &sens.avrg, sens.oxgn.avrg );
                } //app.evt.sens
        }
}
