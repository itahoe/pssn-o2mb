/**
  * @file    mdbs_dev.c
  * @brief   MODBUS Device implementation
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <string.h>
#include "mdbs.h"
#include "mdbs_dev_o2mb.h"
#include "sens.h"
//#include "sys.h"
#include "nvm.h"
#include "app.h"
#include "dev.h"


extern  sens_t          sens;
//extern  dev_conf_t      dev_conf;
extern  dev_t           dev;


/**
  * @brief  
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_coil_read(                         const   size_t                  idx,
                                                uint16_t *              data )
{
        mdbs_err_t      err     = MDBS_ERR_NONE;


        switch( idx )
        {
                case MDBS_COIL_AFE_CTL_INPUT_SHORT:
                        *data   = dev_afe_adc_psw_get( &dev );
                        break;

                case MDBS_COIL_AFE_CTL_UNIPOLAR:
                        *data   = dev_afe_adc_unipolar_get( &dev );
                        break;

                case MDBS_COIL_AFE_CTL_BUFFER_ENABLE:
                        *data   = dev_afe_adc_buffer_enable_get( &dev );
                        break;

                default:
                        err     = MDBS_ERR_ILLEGAL_DATA_ADDRESS;
                        break;
        }

        return( err );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_coil_write(                        const   size_t                  idx,
                                        const   uint16_t *              data )
{
        mdbs_err_t      err     = MDBS_ERR_NONE;


        switch( idx )
        {
                case MDBS_COIL_AFE_CTL_INPUT_SHORT:
                        dev_afe_adc_psw_set( &dev, *data );
                        break;

                case MDBS_COIL_AFE_CTL_UNIPOLAR:
                        dev_afe_adc_unipolar_set( &dev, *data );
                        break;

                case MDBS_COIL_AFE_CTL_BUFFER_ENABLE:
                        dev_afe_adc_buffer_enable_set( &dev, *data );

                        break;

                case MDBS_COIL_MAX:
                        //address exist, read-only register, do nothing
                        break;

                default:
                        err     = MDBS_ERR_ILLEGAL_DATA_ADDRESS;
                        break;
        }

        return( err );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_dinp_read(                         const   size_t                  idx,
                                                uint16_t *              data )
{
        return( MDBS_ERR_ILLEGAL_FUNCTION );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_dinp_write(                        const   size_t                  idx,
                                                uint16_t *              data )
{
        return( MDBS_ERR_ILLEGAL_FUNCTION );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_ireg_read(                         const   size_t                  idx,
                                                uint16_t *              data )
{
        return( MDBS_ERR_ILLEGAL_FUNCTION );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_ireg_write(                        const   size_t                  idx,
                                        const   uint16_t *              data )
{
        return( MDBS_ERR_ILLEGAL_FUNCTION );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_hreg_read(                         const   size_t                  idx,
                                                uint16_t *              data )
{
        mdbs_err_t      err     = MDBS_ERR_NONE;


        switch( idx )
        {
                case MDBS_HREG_DEVICE_ID:
                        *data   = dev.conf->device_id;
                        break;

                case MDBS_HREG_HARDWARE_ID:
                        *data   = dev.conf->hardware_id;
                        break;

                case MDBS_HREG_RESERVED_02:
                        *data   = 0;
                        break;

                case MDBS_HREG_RESERVED_03:
                        *data   = 0;
                        break;

                case MDBS_HREG_FIRMWARE_ID_HI:
                        *data   = dev.conf->firmware_id[ 0];
                        break;

                case MDBS_HREG_FIRMWARE_ID_LO:
                        *data   = dev.conf->firmware_id[ 1];
                        break;

                case MDBS_HREG_RESERVED_06:
                case MDBS_HREG_RESERVED_07:
                        *data   = 0;
                        break;

                case MDBS_HREG_SERIAL_NUM_00:
                        *data   = dev.conf->serial_num[ 0];
                        break;

                case MDBS_HREG_SERIAL_NUM_01:
                        *data   = dev.conf->serial_num[ 1];
                        break;

                case MDBS_HREG_SERIAL_NUM_02:
                        *data   = dev.conf->serial_num[ 2];
                        break;

                case MDBS_HREG_SERIAL_NUM_03:
                        *data   = dev.conf->serial_num[ 3];
                        break;

                case MDBS_HREG_SERIAL_NUM_04:
                        *data   = dev.conf->serial_num[ 4];
                        break;

                case MDBS_HREG_SERIAL_NUM_05:
                        *data   = dev.conf->serial_num[ 5];
                        break;

                case MDBS_HREG_SERIAL_NUM_06:
                        *data   = dev.conf->serial_num[ 6];
                        break;

                case MDBS_HREG_SERIAL_NUM_07:
                        *data   = dev.conf->serial_num[ 7];
                        break;


                case MDBS_HREG_ERR_CODE_LAST:
                        *data   = 0;
                        break;

                case MDBS_HREG_STARTS_COUNTER:
                        *data   = dev.mcu.starts_cnt;
                        break;

                case MDBS_HREG_RESERVED_12:
                case MDBS_HREG_RESERVED_13:
                        *data   = 0;
                        break;

                case MDBS_HREG_ADC_SCALE_mV:
                        *data   = dev.conf->adc_vref_mV;
                        break;

                case MDBS_HREG_ADC_RESOLUTION_BITS:
                        *data   = dev.conf->adc_resolution_bits;
                        break;

                case MDBS_HREG_RESERVED_16:
                case MDBS_HREG_RESERVED_17:
                        *data   = 0;
                        break;


                case MDBS_HREG_MCU_TEMP_CELS:
                        *data   = dev.mcu.degc;
                        break;

                case MDBS_HREG_MCU_VDDA_mV:
                        *data   = dev.mcu.vref_mV;
                        break;

                case MDBS_HREG_RAW2PPM_FV_HI:
                        *data   = sens.oxgn.fv.u16[ 1];
                        break;

                case MDBS_HREG_RAW2PPM_FV_LO:
                        *data   = sens.oxgn.fv.u16[ 0];
                        break;

                case MDBS_HREG_RAW2PPM_FT_HI:
                        *data   = sens.oxgn.ft.u16[ 1];
                        break;

                case MDBS_HREG_RAW2PPM_FT_LO:
                        *data   = sens.oxgn.ft.u16[ 0];
                        break;

                case MDBS_HREG_RAW2PPM_FP_HI:
                        *data   = sens.oxgn.fp.u16[ 1];
                        break;

                case MDBS_HREG_RAW2PPM_FP_LO:
                        *data   = sens.oxgn.fp.u16[ 0];
                        break;


                case MDBS_HREG_SENS_OXGN_PPM_HI:
                        *data   = sens.oxgn.ppm.u16[ 1];
                        break;

                case MDBS_HREG_SENS_OXGN_PPM_LO:
                        *data   = sens.oxgn.ppm.u16[ 0];
                        break;

                case MDBS_HREG_SENS_TEMP_DIGC_HI:
                        *data   = sens.temp.digc.u16[ 1];
                        break;

                case MDBS_HREG_SENS_TEMP_DIGC_LO:
                        *data   = sens.temp.digc.u16[ 0];
                        break;

                case MDBS_HREG_SENS_PRES_HPA_HI:
                        *data   = sens.pres.hPa.u16[ 1];
                        break;

                case MDBS_HREG_SENS_PRES_HPA_LO:
                        *data   = sens.pres.hPa.u16[ 0];
                        break;

                case MDBS_HREG_RESERVED_26:
                case MDBS_HREG_RESERVED_27:
                        *data   = 0;
                        break;


                case MDBS_HREG_SENS_RAW_HI:
                        //*data   = sens.oxgn.raw.u16[ 1];
                        *data   = dev.afe.adc_raw.u16[ 1];
                        break;

                case MDBS_HREG_SENS_RAW_LO:
                        //*data   = sens.oxgn.raw.u16[ 0];
                        *data   = dev.afe.adc_raw.u16[ 0];
                        break;

                case MDBS_HREG_TEMP_RAW_HI:
                        //*data   = sens.temp.raw.u16[ 1];
                        *data   = 0;
                        break;

                case MDBS_HREG_TEMP_RAW_LO:
                        //*data   = sens.temp.raw.u16[ 0];
                        *data   = sens.temp.raw;
                        break;

                case MDBS_HREG_PRES_RAW_HI:
                        *data   = sens.pres.raw.u16[ 1];
                        break;

                case MDBS_HREG_PRES_RAW_LO:
                        *data   = sens.pres.raw.u16[ 0];
                        break;

                case MDBS_HREG_SENS_SLOPE_RAW:
                        *data   = sens.avrg.slope;
                        break;

                case MDBS_HREG_SENS_OFST_RAW:
                        //*data   = sens_oxgn_ofst_get();
                        *data   = 0;
                        break;


                case MDBS_HREG_SENS_TRIM_P0_TIMESTMP_HI:
                        *data   = sens.trim.point[ 0].timestmp.u16[ 1];
                        break;

                case MDBS_HREG_SENS_TRIM_P0_TIMESTMP_LO:
                        *data   = sens.trim.point[ 0].timestmp.u16[ 0];
                        break;

                case MDBS_HREG_SENS_TRIM_P0_OXGN_PPM_HI:
                        *data   = sens.trim.point[ 0].oxgn_ppm.u16[ 1];
                        break;

                case MDBS_HREG_SENS_TRIM_P0_OXGN_PPM_LO:
                        *data   = sens.trim.point[ 0].oxgn_ppm.u16[ 0];
                        break;

                case MDBS_HREG_SENS_TRIM_P0_OXGN_RAW_HI:
                        *data   = sens.trim.point[ 0].oxgn_raw.u16[ 1];
                        break;

                case MDBS_HREG_SENS_TRIM_P0_OXGN_RAW_LO:
                        *data   = sens.trim.point[ 0].oxgn_raw.u16[ 0];
                        break;

                case MDBS_HREG_RESERVED_36:
                case MDBS_HREG_RESERVED_37:
                        *data   = 0;
                        break;


                case MDBS_HREG_SENS_TRIM_P1_TIMESTMP_HI:
                        *data   = sens.trim.point[ 1].timestmp.u16[ 1];
                        break;

                case MDBS_HREG_SENS_TRIM_P1_TIMESTMP_LO:
                        *data   = sens.trim.point[ 1].timestmp.u16[ 0];
                        break;

                case MDBS_HREG_SENS_TRIM_P1_OXGN_PPM_HI:
                        *data   = sens.trim.point[ 1].oxgn_ppm.u16[ 1];
                        break;

                case MDBS_HREG_SENS_TRIM_P1_OXGN_PPM_LO:
                        *data   = sens.trim.point[ 1].oxgn_ppm.u16[ 0];
                        break;

                case MDBS_HREG_SENS_TRIM_P1_OXGN_RAW_HI:
                        *data   = sens.trim.point[ 1].oxgn_raw.u16[ 1];
                        break;

                case MDBS_HREG_SENS_TRIM_P1_OXGN_RAW_LO:
                        *data   = sens.trim.point[ 1].oxgn_raw.u16[ 0];
                        break;

                case MDBS_HREG_RESERVED_3E:
                case MDBS_HREG_RESERVED_3F:
                        *data   = 0;
                        break;

                //V2 CONF
                case MDBS_HREG_CONF_DEVICE_ID:
                        *data   = dev.conf->device_id;
                        break;

                case MDBS_HREG_CONF_HARDWARE_ID:
                        *data   = dev.conf->hardware_id;
                        break;

                case MDBS_HREG_CONF_RESERVED_0102:
                case MDBS_HREG_CONF_RESERVED_0103:
                        *data   = 0;
                        break;

                case MDBS_HREG_CONF_FIRMWARE_ID_HI:
                        *data   = dev.conf->firmware_id[ 0];
                        break;

                case MDBS_HREG_CONF_FIRMWARE_ID_LO:
                        *data   = dev.conf->firmware_id[ 1];
                        break;

                case MDBS_HREG_CONF_RESERVED_0106:
                case MDBS_HREG_CONF_RESERVED_0107:
                        *data   = 0;
                        break;



                case MDBS_HREG_CONF_SERIAL_NUM_00:
                        *data   = dev.conf->serial_num[ 0];
                        break;

                case MDBS_HREG_CONF_SERIAL_NUM_01:
                        *data   = dev.conf->serial_num[ 1];
                        break;

                case MDBS_HREG_CONF_SERIAL_NUM_02:
                        *data   = dev.conf->serial_num[ 2];
                        break;

                case MDBS_HREG_CONF_SERIAL_NUM_03:
                        *data   = dev.conf->serial_num[ 3];
                        break;

                case MDBS_HREG_CONF_SERIAL_NUM_04:
                        *data   = dev.conf->serial_num[ 4];
                        break;

                case MDBS_HREG_CONF_SERIAL_NUM_05:
                        *data   = dev.conf->serial_num[ 5];
                        break;

                case MDBS_HREG_CONF_SERIAL_NUM_06:
                        *data   = dev.conf->serial_num[ 6];
                        break;

                case MDBS_HREG_CONF_SERIAL_NUM_07:
                        *data   = dev.conf->serial_num[ 7];
                        break;

                case MDBS_HREG_CONF_AFE_ADC_SPAN:
                        *data   = dev.conf->adc_vref_mV;
                        break;

                case MDBS_HREG_CONF_AFE_ADC_RESOLUTION:
                        *data   = dev.conf->adc_resolution_bits;
                        break;

                case MDBS_HREG_CONF_RESERVED_0142:
                case MDBS_HREG_CONF_RESERVED_0143:
                        *data   = 0;
                        break;

                case MDBS_HREG_CONF_AFE_BIAS:
                        *data   = dev_afe_bias_get( &dev );
                        break;

                case MDBS_HREG_CONF_RESERVED_0145:
                case MDBS_HREG_CONF_RESERVED_0146:
                case MDBS_HREG_CONF_RESERVED_0147:
                        *data   = 0;
                        break;

                case MDBS_HREG_CONF_AFE_DRIFT_K_TEMP_HI:
                        *data   = sens.drift_k_temp.u16[ 1];
                        break;

                case MDBS_HREG_CONF_AFE_DRIFT_K_TEMP_LO:
                        *data   = sens.drift_k_temp.u16[ 0];
                        break;

                case MDBS_HREG_CONF_RESERVED_014A:
                case MDBS_HREG_CONF_RESERVED_014B:
                        *data   = 0;
                        break;

                case MDBS_HREG_CONF_AFE_DRIFT_K_PRES_HI:
                        *data   = sens.drift_k_pres.u16[ 0];
                        break;

                case MDBS_HREG_CONF_AFE_DRIFT_K_PRES_LO:
                        *data   = sens.drift_k_pres.u16[ 1];
                        break;

                case MDBS_HREG_CONF_RESERVED_014E:
                case MDBS_HREG_CONF_RESERVED_014F:
                        *data   = 0;
                        break;


                case MDBS_HREG_CONF_AD7799_MODE:
                        *data   = dev_afe_adc_mode_get( &dev );
                        break;

                case MDBS_HREG_CONF_AD7799_CONF:
                        *data   = dev_afe_adc_conf_get( &dev );
                        break;

                case MDBS_HREG_CONF_RESERVED_0152:
                case MDBS_HREG_CONF_RESERVED_0153:
                        *data   = 0;
                        break;

                case MDBS_HREG_CONF_AD7799_CHANNEL:
                        *data   = dev_afe_adc_chnl_get( &dev );
                        break;

                case MDBS_HREG_CONF_AD7799_GAIN:
                        *data   = dev_afe_adc_gain_get( &dev );
                        break;

                case MDBS_HREG_CONF_RESERVED_0156:
                case MDBS_HREG_CONF_RESERVED_0157:
                case MDBS_HREG_CONF_RESERVED_0158:
                case MDBS_HREG_CONF_RESERVED_0159:
                case MDBS_HREG_CONF_RESERVED_015A:
                case MDBS_HREG_CONF_RESERVED_015B:
                case MDBS_HREG_CONF_RESERVED_015C:
                case MDBS_HREG_CONF_RESERVED_015D:
                case MDBS_HREG_CONF_RESERVED_015E:
                case MDBS_HREG_CONF_RESERVED_015F:
                        *data   = 0;
                        break;

                //V2 MEAS
                case MDBS_HREG_MEAS_CONCENTRATION_PPM_HI:
                        *data   = sens.oxgn.ppm.u16[ 1];
                        break;

                case MDBS_HREG_MEAS_CONCENTRATION_PPM_LO:
                        *data   = sens.oxgn.ppm.u16[ 0];
                        break;

                case MDBS_HREG_MEAS_TEMPERATURE_DIGC_HI:
                        *data   = sens.temp.digc.u16[ 1];
                        break;

                case MDBS_HREG_MEAS_TEMPERATURE_DIGC_LO:
                        *data   = sens.temp.digc.u16[ 0];
                        break;

                case MDBS_HREG_MEAS_PRESSURE_HPA_HI:
                        *data   = sens.pres.hPa.u16[ 1];
                        break;

                case MDBS_HREG_MEAS_PRESSURE_HPA_LO:
                        *data   = sens.pres.hPa.u16[ 0];
                        break;

                case MDBS_HREG_MEAS_RESERVED_0206:
                case MDBS_HREG_MEAS_RESERVED_0207:
                        *data   = 0;
                        break;

                case MDBS_HREG_MEAS_CONCENTRATION_SLOPE_RAW:
                        *data   = sens.avrg.slope;
                        break;

                case MDBS_HREG_MEAS_SENSOR_OFFSET_RAW:
                        //*data   = sens_oxgn_ofst_get();
                        *data   = 0;
                        break;

                case MDBS_HREG_MEAS_RESERVED_020A:
                case MDBS_HREG_MEAS_RESERVED_020B:

                case MDBS_HREG_MEAS_MCU_TEMP_DIGC:
                        *data   = dev.mcu.degc;
                        break;

                case MDBS_HREG_MEAS_MCU_VDDA_mV:
                        *data   = dev.mcu.vref_mV;
                        break;

                case MDBS_HREG_MEAS_RESERVED_020E:
                case MDBS_HREG_MEAS_RESERVED_020F:
                        *data   = 0;
                        break;

                case MDBS_HREG_MEAS_ADC_RAW_HI:
                        //*data   = sens.oxgn.raw.u16[ 1];
                        *data   = dev.afe.adc_raw.u16[ 1];
                        break;

                case MDBS_HREG_MEAS_ADC_RAW_LO:
                        //*data   = sens.oxgn.raw.u16[ 0];
                        *data   = dev.afe.adc_raw.u16[ 0];
                        break;

                case MDBS_HREG_MEAS_TEMPERATURE_RAW_HI:
                        //*data   = sens.temp.raw.u16[ 1];
                        *data   = 0;
                        break;

                case MDBS_HREG_MEAS_TEMPERATURE_RAW_LO:
                        //*data   = sens.temp.raw.u16[ 0];
                        *data   = sens.temp.raw;
                        break;

                case MDBS_HREG_MEAS_PRESSURE_RAW_HI:
                        *data   = sens.pres.raw.u16[ 1];
                        break;

                case MDBS_HREG_MEAS_PRESSURE_RAW_LO:
                        *data   = sens.pres.raw.u16[ 0];
                        break;

                case MDBS_HREG_MEAS_RESERVED_0216:
                case MDBS_HREG_MEAS_RESERVED_0217:
                case MDBS_HREG_MEAS_RESERVED_0218:
                case MDBS_HREG_MEAS_RESERVED_0219:
                case MDBS_HREG_MEAS_RESERVED_021A:
                case MDBS_HREG_MEAS_RESERVED_021B:
                case MDBS_HREG_MEAS_RESERVED_021C:
                case MDBS_HREG_MEAS_RESERVED_021D:
                case MDBS_HREG_MEAS_RESERVED_021E:
                case MDBS_HREG_MEAS_RESERVED_021F:
                        *data   = 0;
                        break;

                //V2 STS
                case MDBS_HREG_STS_LAST_ERROR:
                        *data   = 0;
                        break;

                case MDBS_HREG_STS_STARTS_COUNTER:
                        *data   = dev.mcu.starts_cnt;
                        break;

                case MDBS_HREG_STS_RESERVED_0302:
                case MDBS_HREG_STS_RESERVED_0303:
                case MDBS_HREG_STS_RESERVED_0304:
                case MDBS_HREG_STS_RESERVED_0305:
                case MDBS_HREG_STS_RESERVED_0306:
                case MDBS_HREG_STS_RESERVED_0307:
                case MDBS_HREG_STS_RESERVED_0308:
                case MDBS_HREG_STS_RESERVED_0309:
                case MDBS_HREG_STS_RESERVED_030A:
                case MDBS_HREG_STS_RESERVED_030B:
                case MDBS_HREG_STS_RESERVED_030C:
                case MDBS_HREG_STS_RESERVED_030D:
                case MDBS_HREG_STS_RESERVED_030E:
                case MDBS_HREG_STS_RESERVED_030F:
                        *data   = 0;
                        break;

                default:
                        err     = MDBS_ERR_ILLEGAL_DATA_ADDRESS;
                        break;
        }

        return( err );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_hreg_write(                        const   size_t                  idx,
                                        const   uint16_t *              data )
{
        mdbs_err_t      err     = MDBS_ERR_NONE;


        switch( idx )
        {
                case MDBS_HREG_DEVICE_ID:
                case MDBS_HREG_HARDWARE_ID:
                case MDBS_HREG_RESERVED_02:
                case MDBS_HREG_RESERVED_03:
                case MDBS_HREG_FIRMWARE_ID_HI:
                case MDBS_HREG_FIRMWARE_ID_LO:
                case MDBS_HREG_RESERVED_06:
                case MDBS_HREG_RESERVED_07:
                case MDBS_HREG_SERIAL_NUM_00:
                case MDBS_HREG_SERIAL_NUM_01:
                case MDBS_HREG_SERIAL_NUM_02:
                case MDBS_HREG_SERIAL_NUM_03:
                case MDBS_HREG_SERIAL_NUM_04:
                case MDBS_HREG_SERIAL_NUM_05:
                case MDBS_HREG_SERIAL_NUM_06:
                case MDBS_HREG_SERIAL_NUM_07:
                        break;  //address exist, read-only register, do nothing

                case MDBS_HREG_ERR_CODE_LAST:
                        break;  //address exist, read-only register, do nothing

                case MDBS_HREG_STARTS_COUNTER:
                        dev.mcu.starts_cnt      = *data;
                        break;

                case MDBS_HREG_RESERVED_12:
                case MDBS_HREG_RESERVED_13:
                        //address exist, read-only register, do nothing
                        break;

                case MDBS_HREG_ADC_SCALE_mV:
                case MDBS_HREG_ADC_RESOLUTION_BITS:
                        //address exist, read-only register, do nothing
                        break;

                case MDBS_HREG_RESERVED_16:
                case MDBS_HREG_RESERVED_17:
                        //address exist, read-only register, do nothing
                        break;

                case MDBS_HREG_MCU_TEMP_CELS:
                case MDBS_HREG_MCU_VDDA_mV:
                case MDBS_HREG_RAW2PPM_FV_HI:
                case MDBS_HREG_RAW2PPM_FV_LO:
                case MDBS_HREG_RAW2PPM_FT_HI:
                case MDBS_HREG_RAW2PPM_FT_LO:
                case MDBS_HREG_RAW2PPM_FP_HI:
                case MDBS_HREG_RAW2PPM_FP_LO:
                        //address exist, read-only register, do nothing
                        break;


                case MDBS_HREG_SENS_OXGN_PPM_LO:
                case MDBS_HREG_SENS_OXGN_PPM_HI:
                case MDBS_HREG_SENS_TEMP_DIGC_HI:
                case MDBS_HREG_SENS_TEMP_DIGC_LO:
                case MDBS_HREG_SENS_PRES_HPA_HI:
                case MDBS_HREG_SENS_PRES_HPA_LO:
                case MDBS_HREG_RESERVED_26:
                case MDBS_HREG_RESERVED_27:
                        //address exist, read-only register, do nothing
                        break;

                case MDBS_HREG_SENS_RAW_HI:
                case MDBS_HREG_SENS_RAW_LO:
                case MDBS_HREG_TEMP_RAW_HI:
                case MDBS_HREG_TEMP_RAW_LO:
                case MDBS_HREG_PRES_RAW_HI:
                case MDBS_HREG_PRES_RAW_LO:
                        //address exist, read-only register, do nothing
                        break;

                case MDBS_HREG_SENS_SLOPE_RAW:
                        //address exist, read-only register, do nothing
                        break;

                case MDBS_HREG_SENS_OFST_RAW:
                        dev_afe_bias_set( &dev, *data );
                        //nvm_write16( NVM_ADDR_SENS_OFST_RAW, (uint16_t *) data, 1 );
                        break;


                case MDBS_HREG_SENS_TRIM_P0_TIMESTMP_HI:
                        sens.trim.point[ 0].timestmp.u16[ 1]    = *data;
                        break;

                case MDBS_HREG_SENS_TRIM_P0_TIMESTMP_LO:
                        sens.trim.point[ 0].timestmp.u16[ 0]    = *data;
                        break;

                case MDBS_HREG_SENS_TRIM_P0_OXGN_PPM_HI:
                        sens.trim.point[ 0].oxgn_ppm.u16[ 1]    = *data;
                        break;

                case MDBS_HREG_SENS_TRIM_P0_OXGN_PPM_LO:
                        sens.trim.point[ 0].oxgn_ppm.u16[ 0]    = *data;
                        sens.trim.point[ 0].oxgn_raw.i32        = sens.oxgn.raw.i32;
                        sens.trim.point[ 0].temp_digc.f32       = sens.temp.digc.f32;
                        sens.trim.point[ 0].pres_raw.i32        = sens.pres.raw.i32;
                        sens_trim_restore( &sens.trim );
                        nvm_write16( NVM_ADDR_TRIM_P0_TIMESTMP_HI, &sens.trim.point[ 0].timestmp.u16[ 1], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P0_TIMESTMP_LO, &sens.trim.point[ 0].timestmp.u16[ 0], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P0_OXGN_PPM_HI, &sens.trim.point[ 0].oxgn_ppm.u16[ 1], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P0_OXGN_PPM_LO, &sens.trim.point[ 0].oxgn_ppm.u16[ 0], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P0_OXGN_RAW_HI, &sens.trim.point[ 0].oxgn_raw.u16[ 1], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P0_OXGN_RAW_LO, &sens.trim.point[ 0].oxgn_raw.u16[ 0], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P0_TEMP_DIGC_HI, &sens.trim.point[ 0].temp_digc.u16[ 1], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P0_TEMP_DIGC_LO, &sens.trim.point[ 0].temp_digc.u16[ 0], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P0_PRES_RAW_HI, &sens.trim.point[ 0].pres_raw.u16[ 1], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P0_PRES_RAW_LO, &sens.trim.point[ 0].pres_raw.u16[ 0], 1 );
                        break;

                case MDBS_HREG_SENS_TRIM_P0_OXGN_RAW_HI:
                case MDBS_HREG_SENS_TRIM_P0_OXGN_RAW_LO:
                case MDBS_HREG_RESERVED_36:
                case MDBS_HREG_RESERVED_37:
                        //address exist, read-only register, do nothing
                        break;


                case MDBS_HREG_SENS_TRIM_P1_TIMESTMP_HI:
                        sens.trim.point[ 1].timestmp.u16[ 1]    = *data;
                        break;

                case MDBS_HREG_SENS_TRIM_P1_TIMESTMP_LO:
                        sens.trim.point[ 1].timestmp.u16[ 0]    = *data;
                        break;

                case MDBS_HREG_SENS_TRIM_P1_OXGN_PPM_HI:
                        sens.trim.point[ 1].oxgn_ppm.u16[ 1]    = *data;
                        break;

                case MDBS_HREG_SENS_TRIM_P1_OXGN_PPM_LO:
                        sens.trim.point[ 1].oxgn_ppm.u16[ 0]    = *data;
                        sens.trim.point[ 1].oxgn_raw.i32        = sens.oxgn.raw.i32;
                        sens.trim.point[ 1].temp_digc.f32       = sens.temp.digc.f32;
                        sens.trim.point[ 1].pres_raw.i32        = sens.pres.raw.i32;
                        sens_trim_restore( &sens.trim );
                        nvm_write16( NVM_ADDR_TRIM_P1_TIMESTMP_HI, &sens.trim.point[ 1].timestmp.u16[ 1], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P1_TIMESTMP_LO, &sens.trim.point[ 1].timestmp.u16[ 0], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P1_OXGN_PPM_HI, &sens.trim.point[ 1].oxgn_ppm.u16[ 1], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P1_OXGN_PPM_LO, &sens.trim.point[ 1].oxgn_ppm.u16[ 0], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P1_OXGN_RAW_HI, &sens.trim.point[ 1].oxgn_raw.u16[ 1], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P1_OXGN_RAW_LO, &sens.trim.point[ 1].oxgn_raw.u16[ 0], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P1_TEMP_DIGC_HI, &sens.trim.point[ 1].temp_digc.u16[ 1], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P1_TEMP_DIGC_LO, &sens.trim.point[ 1].temp_digc.u16[ 0], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P1_PRES_RAW_HI, &sens.trim.point[ 1].pres_raw.u16[ 1], 1 );
                        nvm_write16( NVM_ADDR_TRIM_P1_PRES_RAW_LO, &sens.trim.point[ 1].pres_raw.u16[ 0], 1 );
                        break;

                case MDBS_HREG_SENS_TRIM_P1_OXGN_RAW_HI:
                case MDBS_HREG_SENS_TRIM_P1_OXGN_RAW_LO:
                case MDBS_HREG_RESERVED_3E:
                case MDBS_HREG_RESERVED_3F:
                        break;  //address exist, read-only register, do nothing


        //V2 CONF
                case MDBS_HREG_CONF_DEVICE_ID:
                case MDBS_HREG_CONF_HARDWARE_ID:
                case MDBS_HREG_CONF_RESERVED_0102:
                case MDBS_HREG_CONF_RESERVED_0103:
                case MDBS_HREG_CONF_FIRMWARE_ID_HI:
                case MDBS_HREG_CONF_FIRMWARE_ID_LO:
                case MDBS_HREG_CONF_RESERVED_0106:
                case MDBS_HREG_CONF_RESERVED_0107:
                case MDBS_HREG_CONF_SERIAL_NUM_00:
                case MDBS_HREG_CONF_SERIAL_NUM_01:
                case MDBS_HREG_CONF_SERIAL_NUM_02:
                case MDBS_HREG_CONF_SERIAL_NUM_03:
                case MDBS_HREG_CONF_SERIAL_NUM_04:
                case MDBS_HREG_CONF_SERIAL_NUM_05:
                case MDBS_HREG_CONF_SERIAL_NUM_06:
                case MDBS_HREG_CONF_SERIAL_NUM_07:
                case MDBS_HREG_CONF_AFE_ADC_SPAN:
                case MDBS_HREG_CONF_AFE_ADC_RESOLUTION:
                case MDBS_HREG_CONF_RESERVED_0142:
                case MDBS_HREG_CONF_RESERVED_0143:
                        break;  //address exist, read-only register, do nothing

                case MDBS_HREG_CONF_AFE_BIAS:
                        dev_afe_bias_set( &dev, *data );
                        //nvm_write16( NVM_ADDR_SENS_OFST_RAW, (uint16_t *) data, 1 );
                        break;

                case MDBS_HREG_CONF_RESERVED_0145:
                case MDBS_HREG_CONF_RESERVED_0146:
                case MDBS_HREG_CONF_RESERVED_0147:
                        break;  //address exist, read-only register, do nothing

                case MDBS_HREG_CONF_AFE_DRIFT_K_TEMP_HI:
                        sens.drift_k_temp.u16[ 1]               = *data;
                        break;

                case MDBS_HREG_CONF_AFE_DRIFT_K_TEMP_LO:
                        sens.drift_k_temp.u16[ 0]               = *data;
                        nvm_write16( NVM_ADDR_AFE_K_TEMP_DRIFT_HI,  &sens.drift_k_temp.u16[ 1],     1 );
                        nvm_write16( NVM_ADDR_AFE_K_TEMP_DRIFT_LO,  &sens.drift_k_temp.u16[ 0],     1 );
                        break;

                case MDBS_HREG_CONF_RESERVED_014A:
                case MDBS_HREG_CONF_RESERVED_014B:
                        break;  //address exist, read-only register, do nothing

                case MDBS_HREG_CONF_AFE_DRIFT_K_PRES_HI:
                        sens.drift_k_pres.u16[ 0]               = *data;
                        break;

                case MDBS_HREG_CONF_AFE_DRIFT_K_PRES_LO:
                        sens.drift_k_pres.u16[ 1]               = *data;
                        nvm_write16( NVM_ADDR_AFE_K_PRES_DRIFT_HI,  &sens.drift_k_pres.u16[ 1],     1 );
                        nvm_write16( NVM_ADDR_AFE_K_PRES_DRIFT_LO,  &sens.drift_k_pres.u16[ 0],     1 );
                        break;

                case MDBS_HREG_CONF_RESERVED_014E:
                case MDBS_HREG_CONF_RESERVED_014F:
                        break;  //address exist, read-only register, do nothing

                case MDBS_HREG_CONF_AD7799_MODE:
                        dev_afe_adc_mode_set( &dev, *data );
                        //dev_afe_adc_mode_set( *data );
                        //dev.ad7799.mode.set( *data );
                        break;

                case MDBS_HREG_CONF_AD7799_CONF:
                        dev_afe_adc_conf_set( &dev, *data );
                        break;

                case MDBS_HREG_CONF_RESERVED_0152:
                case MDBS_HREG_CONF_RESERVED_0153:
                        break;  //address exist, read-only register, do nothing

                case MDBS_HREG_CONF_AD7799_CHANNEL:
                        dev_afe_adc_chnl_set( &dev, *data );
                        break;

                case MDBS_HREG_CONF_AD7799_GAIN:
                        dev_afe_adc_gain_set( &dev, *data );
                        break;

                case MDBS_HREG_CONF_RESERVED_0156:
                case MDBS_HREG_CONF_RESERVED_0157:
                case MDBS_HREG_CONF_RESERVED_0158:
                case MDBS_HREG_CONF_RESERVED_0159:
                case MDBS_HREG_CONF_RESERVED_015A:
                case MDBS_HREG_CONF_RESERVED_015B:
                case MDBS_HREG_CONF_RESERVED_015C:
                case MDBS_HREG_CONF_RESERVED_015D:
                case MDBS_HREG_CONF_RESERVED_015E:
                case MDBS_HREG_CONF_RESERVED_015F:
                        break;  //address exist, read-only register, do nothing


                //V2 MEAS
                case MDBS_HREG_MEAS_CONCENTRATION_PPM_HI:
                case MDBS_HREG_MEAS_CONCENTRATION_PPM_LO:
                case MDBS_HREG_MEAS_TEMPERATURE_DIGC_HI:
                case MDBS_HREG_MEAS_TEMPERATURE_DIGC_LO:
                case MDBS_HREG_MEAS_PRESSURE_HPA_HI:
                case MDBS_HREG_MEAS_PRESSURE_HPA_LO:
                case MDBS_HREG_MEAS_RESERVED_0206:
                case MDBS_HREG_MEAS_RESERVED_0207:
                case MDBS_HREG_MEAS_CONCENTRATION_SLOPE_RAW:
                case MDBS_HREG_MEAS_SENSOR_OFFSET_RAW:
                case MDBS_HREG_MEAS_RESERVED_020A:
                case MDBS_HREG_MEAS_RESERVED_020B:
                case MDBS_HREG_MEAS_MCU_TEMP_DIGC:
                case MDBS_HREG_MEAS_MCU_VDDA_mV:
                case MDBS_HREG_MEAS_RESERVED_020E:
                case MDBS_HREG_MEAS_RESERVED_020F:
                case MDBS_HREG_MEAS_ADC_RAW_HI:
                case MDBS_HREG_MEAS_ADC_RAW_LO:
                case MDBS_HREG_MEAS_TEMPERATURE_RAW_HI:
                case MDBS_HREG_MEAS_TEMPERATURE_RAW_LO:
                case MDBS_HREG_MEAS_PRESSURE_RAW_HI:
                case MDBS_HREG_MEAS_PRESSURE_RAW_LO:
                case MDBS_HREG_MEAS_RESERVED_0216:
                case MDBS_HREG_MEAS_RESERVED_0217:
                case MDBS_HREG_MEAS_RESERVED_0218:
                case MDBS_HREG_MEAS_RESERVED_0219:
                case MDBS_HREG_MEAS_RESERVED_021A:
                case MDBS_HREG_MEAS_RESERVED_021B:
                case MDBS_HREG_MEAS_RESERVED_021C:
                case MDBS_HREG_MEAS_RESERVED_021D:
                case MDBS_HREG_MEAS_RESERVED_021E:
                case MDBS_HREG_MEAS_RESERVED_021F:
                        break;  //address exist, read-only register, do nothing


                //V2 STS
                case MDBS_HREG_STS_LAST_ERROR:
                        break;  //address exist, read-only register, do nothing

                case MDBS_HREG_STS_STARTS_COUNTER:
                        dev.mcu.starts_cnt      = *data;
                        break;

                case MDBS_HREG_STS_RESERVED_0302:
                case MDBS_HREG_STS_RESERVED_0303:
                case MDBS_HREG_STS_RESERVED_0304:
                case MDBS_HREG_STS_RESERVED_0305:
                case MDBS_HREG_STS_RESERVED_0306:
                case MDBS_HREG_STS_RESERVED_0307:
                case MDBS_HREG_STS_RESERVED_0308:
                case MDBS_HREG_STS_RESERVED_0309:
                case MDBS_HREG_STS_RESERVED_030A:
                case MDBS_HREG_STS_RESERVED_030B:
                case MDBS_HREG_STS_RESERVED_030C:
                case MDBS_HREG_STS_RESERVED_030D:
                case MDBS_HREG_STS_RESERVED_030E:
                case MDBS_HREG_STS_RESERVED_030F:
                        break;  //address exist, read-only register, do nothing

                default:
                        err     = MDBS_ERR_ILLEGAL_DATA_ADDRESS;
                        break;
        }

        return( err );
}
