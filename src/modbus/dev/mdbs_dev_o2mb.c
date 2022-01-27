/**
  * @file    mdbs_dev.c
  * @brief   MODBUS Device implementation
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <string.h>
#include "mdbs.h"
#include "mdbs_dev_o2mb.h"
//#include "sens.h"
//#include "sys.h"
#include "nvm.h"
#include "app.h"
#include "dev.h"


extern  dev_t           dev;
//extern  sens_t          sens;


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
    mdbs_err_t      err = MDBS_ERR_NONE;
    dev_trim_t *    zero    = &( dev.sens.zero );
    dev_trim_t *    span    = &( dev.sens.span );



    switch( idx )
    {
        case MDBS_HREG_V1_DEVICE_ID:                *data   = dev.conf->device_id;          break;
        case MDBS_HREG_V1_HARDWARE_ID:              *data   = dev.conf->hardware_id;        break;
        case MDBS_HREG_V1_RESERVED_02:              *data   = 0;                            break;
        case MDBS_HREG_V1_RESERVED_03:              *data   = 0;                            break;
        case MDBS_HREG_V1_FIRMWARE_ID_HI:           *data   = dev.conf->firmware_id[ 0];    break;
        case MDBS_HREG_V1_FIRMWARE_ID_LO:           *data   = dev.conf->firmware_id[ 1];    break;
        case MDBS_HREG_V1_RESERVED_06:              *data   = 0;                            break;
        case MDBS_HREG_V1_RESERVED_07:              *data   = 0;                            break;
        case MDBS_HREG_V1_SERIAL_NUM_00:            *data   = dev.conf->serial_num[ 0];     break;
        case MDBS_HREG_V1_SERIAL_NUM_01:            *data   = dev.conf->serial_num[ 1];     break;
        case MDBS_HREG_V1_SERIAL_NUM_02:            *data   = dev.conf->serial_num[ 2];     break;
        case MDBS_HREG_V1_SERIAL_NUM_03:            *data   = dev.conf->serial_num[ 3];     break;
        case MDBS_HREG_V1_SERIAL_NUM_04:            *data   = dev.conf->serial_num[ 4];     break;
        case MDBS_HREG_V1_SERIAL_NUM_05:            *data   = dev.conf->serial_num[ 5];     break;
        case MDBS_HREG_V1_SERIAL_NUM_06:            *data   = dev.conf->serial_num[ 6];     break;
        case MDBS_HREG_V1_SERIAL_NUM_07:            *data   = dev.conf->serial_num[ 7];     break;
        case MDBS_HREG_V1_ERR_CODE_LAST:            *data   = 0;                            break;
        case MDBS_HREG_V1_STARTS_COUNTER:           *data   = dev.starts_cnt;               break;
        case MDBS_HREG_V1_RESERVED_12:              *data   = 0;                            break;
        case MDBS_HREG_V1_RESERVED_13:              *data   = 0;                            break;
        case MDBS_HREG_V1_ADC_SCALE_mV:             *data   = dev.conf->adc_vref_mV;        break;
        case MDBS_HREG_V1_ADC_RESOLUTION_BITS:      *data   = dev.conf->adc_resolution_bits;break;
        case MDBS_HREG_V1_RESERVED_16:              *data   = 0;                            break;
        case MDBS_HREG_V1_RESERVED_17:              *data   = 0;                            break;
        case MDBS_HREG_V1_MCU_TEMP_CELS:            *data   = dev.mcu.degc;                 break;
        case MDBS_HREG_V1_MCU_VDDA_mV:              *data   = dev.mcu.vref_mV;              break;
        case MDBS_HREG_V1_RAW2PPM_FV_HI:            *data   = 0;                            break;
        case MDBS_HREG_V1_RAW2PPM_FV_LO:            *data   = 0;                            break;
        case MDBS_HREG_V1_RAW2PPM_FT_HI:            *data   = 0;                            break;
        case MDBS_HREG_V1_RAW2PPM_FT_LO:            *data   = 0;                            break;
        case MDBS_HREG_V1_RAW2PPM_FP_HI:            *data   = 0;                            break;
        case MDBS_HREG_V1_RAW2PPM_FP_LO:            *data   = 0;                            break;
        case MDBS_HREG_V1_SENS_OXGN_PPM_HI:         *data   = dev.sens.meas.ppm.u16[ 1];        break;
        case MDBS_HREG_V1_SENS_OXGN_PPM_LO:         *data   = dev.sens.meas.ppm.u16[ 0];        break;
        case MDBS_HREG_V1_SENS_TEMP_DIGC_HI:        *data   = dev.sens.temp.digc.u16[ 1];       break;
        case MDBS_HREG_V1_SENS_TEMP_DIGC_LO:        *data   = dev.sens.temp.digc.u16[ 0];       break;
        case MDBS_HREG_V1_SENS_PRES_HPA_HI:         *data   = dev.sens.pres.hPa.u16[ 1];        break;
        case MDBS_HREG_V1_SENS_PRES_HPA_LO:         *data   = dev.sens.pres.hPa.u16[ 0];        break;
        case MDBS_HREG_V1_RESERVED_26:              *data   = 0;                            break;
        case MDBS_HREG_V1_RESERVED_27:              *data   = 0;                            break;
        case MDBS_HREG_V1_SENS_RAW_HI:              *data   = dev.sens.meas.raw.u16[ 1];    break;
        case MDBS_HREG_V1_SENS_RAW_LO:              *data   = dev.sens.meas.raw.u16[ 0];    break;
        case MDBS_HREG_V1_TEMP_RAW_HI:              *data   = 0;                            break;
        case MDBS_HREG_V1_TEMP_RAW_LO:              *data   = dev.sens.temp.raw;                break;
        case MDBS_HREG_V1_PRES_RAW_HI:              *data   = dev.sens.pres.raw.u16[ 1];        break;
        case MDBS_HREG_V1_PRES_RAW_LO:              *data   = dev.sens.pres.raw.u16[ 0];        break;
        case MDBS_HREG_V1_SENS_MEAS_RMSE:           *data = dev.sens.meas.rmse;     break;
        case MDBS_HREG_V1_SENS_OFST_RAW:            *data   = 0;                    break;
        case MDBS_HREG_V1_SENS_ZERO_TIMESTMP_HI:    *data  = zero->timestmp.u16[ 1];break;
        case MDBS_HREG_V1_SENS_ZERO_TIMESTMP_LO:    *data  = zero->timestmp.u16[ 0];break;
        case MDBS_HREG_V1_SENS_ZERO_OXGN_PPM_HI:    *data  = zero->ppm.u16[ 1];     break;
        case MDBS_HREG_V1_SENS_ZERO_OXGN_PPM_LO:    *data  = zero->ppm.u16[ 0];     break;
        case MDBS_HREG_V1_SENS_ZERO_OXGN_RAW_HI:    *data  = zero->raw.u16[ 1];     break;
        case MDBS_HREG_V1_SENS_ZERO_OXGN_RAW_LO:    *data  = zero->raw.u16[ 0];     break;
        case MDBS_HREG_V1_RESERVED_36:              *data  = 0;                     break;
        case MDBS_HREG_V1_RESERVED_37:              *data  = 0;                     break;
        case MDBS_HREG_V1_SENS_SPAN_TIMESTMP_HI:    *data  = span->timestmp.u16[ 1];break;
        case MDBS_HREG_V1_SENS_SPAN_TIMESTMP_LO:    *data  = span->timestmp.u16[ 0];break;
        case MDBS_HREG_V1_SENS_SPAN_OXGN_PPM_HI:    *data  = span->ppm.u16[ 1];     break;
        case MDBS_HREG_V1_SENS_SPAN_OXGN_PPM_LO:    *data  = span->ppm.u16[ 0];     break;
        case MDBS_HREG_V1_SENS_SPAN_OXGN_RAW_HI:    *data  = span->raw.u16[ 1];     break;
        case MDBS_HREG_V1_SENS_SPAN_OXGN_RAW_LO:    *data  = span->raw.u16[ 0];     break;
        case MDBS_HREG_V1_RESERVED_3E:              *data   = 0;                    break;
        case MDBS_HREG_V1_RESERVED_3F:              *data   = 0;                    break;
        //V2 CONF
        case MDBS_HREG_CONF_DEVICE_ID:          *data = dev.conf->device_id;    break;
        case MDBS_HREG_CONF_HARDWARE_ID:        *data = dev.conf->hardware_id;  break;
        case MDBS_HREG_CONF_RESERVED_0102:      *data = 0;                      break;
        case MDBS_HREG_CONF_RESERVED_0103:      *data = 0;                      break;
        case MDBS_HREG_CONF_FIRMWARE_ID_HI:     *data = dev.conf->firmware_id[ 0];  break;
        case MDBS_HREG_CONF_FIRMWARE_ID_LO:     *data = dev.conf->firmware_id[ 1];  break;
        case MDBS_HREG_RESERVED_0106:           *data = 0;                          break;
        case MDBS_HREG_RESERVED_0107:           *data = 0;                          break;
        case MDBS_HREG_CONF_SERIAL_NUM_00:      *data = dev.conf->serial_num[ 0];   break;
        case MDBS_HREG_CONF_SERIAL_NUM_01:      *data = dev.conf->serial_num[ 1];   break;
        case MDBS_HREG_CONF_SERIAL_NUM_02:      *data = dev.conf->serial_num[ 2];   break;
        case MDBS_HREG_CONF_SERIAL_NUM_03:      *data = dev.conf->serial_num[ 3];   break;
        case MDBS_HREG_CONF_SERIAL_NUM_04:      *data = dev.conf->serial_num[ 4];   break;
        case MDBS_HREG_CONF_SERIAL_NUM_05:      *data = dev.conf->serial_num[ 5];   break;
        case MDBS_HREG_CONF_SERIAL_NUM_06:      *data = dev.conf->serial_num[ 6];   break;
        case MDBS_HREG_CONF_SERIAL_NUM_07:      *data = dev.conf->serial_num[ 7];   break;
        case MDBS_HREG_CONF_AFE_ADC_SPAN:       *data = dev.conf->adc_vref_mV;      break;
        case MDBS_HREG_CONF_AFE_ADC_RESOLUTION: *data = dev.conf->adc_resolution_bits; break;
        case MDBS_HREG_RESERVED_0142:           *data = 0;                          break;
        case MDBS_HREG_RESERVED_0143:           *data = 0;                          break;
        case MDBS_HREG_CONF_AFE_BIAS:           *data = dev_afe_bias_get( &dev );   break;
        case MDBS_HREG_RESERVED_0145:           *data = 0;                          break;
        case MDBS_HREG_RESERVED_0146:           *data = 0;                          break;
        case MDBS_HREG_RESERVED_0147:           *data = 0;                          break;
        case MDBS_HREG_CONF_AFE_DRIFT_K_TEMP_HI: *data = dev.sens.drift_k_temp.u16[ 1]; break;
        case MDBS_HREG_CONF_AFE_DRIFT_K_TEMP_LO: *data = dev.sens.drift_k_temp.u16[ 0]; break;
        case MDBS_HREG_RESERVED_014A:           *data = 0;                          break;
        case MDBS_HREG_RESERVED_014B:           *data = 0;                          break;
        case MDBS_HREG_CONF_AFE_DRIFT_K_PRES_HI: *data = dev.sens.drift_k_pres.u16[ 0]; break;
        case MDBS_HREG_CONF_AFE_DRIFT_K_PRES_LO: *data = dev.sens.drift_k_pres.u16[ 1]; break;
        case MDBS_HREG_RESERVED_014E:           *data = 0;                          break;
        case MDBS_HREG_RESERVED_014F:           *data = 0;                          break;
        case MDBS_HREG_CONF_AD7799_MODE:        *data = dev_afe_adc_mode_get( &dev ); break;
        case MDBS_HREG_CONF_AD7799_CONF:        *data = dev_afe_adc_conf_get( &dev ); break;
        case MDBS_HREG_RESERVED_0152:           *data = 0;                          break;
        case MDBS_HREG_RESERVED_0153:           *data = 0;                          break;
        case MDBS_HREG_CONF_AD7799_CHANNEL:     *data = dev_afe_adc_chnl_get( &dev ); break;
        case MDBS_HREG_CONF_AD7799_GAIN:        *data = dev_afe_adc_gain_get( &dev ); break;
        case MDBS_HREG_RESERVED_0156:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_0157:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_0158:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_0159:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_015A:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_015B:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_015C:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_015D:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_015E:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_015F:           *data = 0;                      break;
        //V2 MEAS
        case MDBS_HREG_SENS_MEAS_PPM_HI:        *data = dev.sens.meas.ppm.u16[ 1];  break;
        case MDBS_HREG_SENS_MEAS_PPM_LO:        *data = dev.sens.meas.ppm.u16[ 0];  break;
        case MDBS_HREG_SENS_TEMP_DIGC_HI:       *data = dev.sens.temp.digc.u16[ 1]; break;
        case MDBS_HREG_SENS_TEMP_DIGC_LO:       *data = dev.sens.temp.digc.u16[ 0]; break;
        case MDBS_HREG_SENS_PRES_HPA_HI:        *data = dev.sens.pres.hPa.u16[ 1];  break;
        case MDBS_HREG_SENS_PRES_HPA_LO:        *data = dev.sens.pres.hPa.u16[ 0];  break;
        case MDBS_HREG_SENS_RESERVED_0206:      *data = 0;                          break;
        case MDBS_HREG_SENS_RESERVED_0207:      *data = 0;                          break;
        case MDBS_HREG_SENS_MEAS_RMSE:          *data = dev.sens.meas.rmse;         break;
        case MDBS_HREG_SENS_MEAS_SENSOR_OFFSET_RAW: *data   = 0;                    break;
        case MDBS_HREG_SENS_RESERVED_020A:      *data = 0;                          break;
        case MDBS_HREG_SENS_RESERVED_020B:      *data = 0;                          break;
        case MDBS_HREG_SENS_MCU_TEMP_DIGC:      *data = dev.mcu.degc;               break;
        case MDBS_HREG_SENS_MCU_VDDA_mV:        *data = dev.mcu.vref_mV;            break;
        case MDBS_HREG_SENS_RESERVED_020E:      *data = 0;                          break;
        case MDBS_HREG_SENS_RESERVED_020F:      *data = 0;                          break;
        case MDBS_HREG_SENS_MEAS_ADC_RAW_HI:    *data = dev.sens.meas.raw.u16[ 1];  break;
        case MDBS_HREG_SENS_MEAS_ADC_RAW_LO:    *data = dev.sens.meas.raw.u16[ 0];  break;
        case MDBS_HREG_SENS_MEAS_TEMP_RAW_HI:   *data = 0;                          break;
        case MDBS_HREG_SENS_MEAS_TEMP_RAW_LO:   *data = dev.sens.temp.raw;          break;
        case MDBS_HREG_SENS_MEAS_PRES_RAW_HI:   *data = dev.sens.pres.raw.u16[ 1];  break;
        case MDBS_HREG_SENS_MEAS_PRES_RAW_LO:   *data = dev.sens.pres.raw.u16[ 0];  break;
        case MDBS_HREG_RESERVED_0216:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_0217:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_0218:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_0219:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_021A:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_021B:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_021C:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_021D:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_021E:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_021F:           *data = 0;                      break;
        case MDBS_HREG_SENS_LPF_FCUT:           *data = dev.sens.lpf.fcut;      break;
        case MDBS_HREG_SENS_LPF_ORDER:          *data = dev.sens.lpf.order;     break;
        case MDBS_HREG_RESERVED_0222:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_0223:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_0224:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_0225:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_0226:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_0227:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_0228:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_0229:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_022A:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_022B:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_022C:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_022D:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_022E:           *data = 0;                      break;
        case MDBS_HREG_RESERVED_022F:           *data = 0;                      break;

        //V2 STS
        case MDBS_HREG_STS_LAST_ERROR:          *data = 0;                            break;
        case MDBS_HREG_STS_STARTS_COUNTER:      *data = dev.starts_cnt;               break;
        case MDBS_HREG_RESERVED_0302:           *data = 0;                            break;
        case MDBS_HREG_RESERVED_0303:           *data = 0;                            break;
        case MDBS_HREG_RESERVED_0304:           *data = 0;                            break;
        case MDBS_HREG_RESERVED_0305:           *data = 0;                            break;
        case MDBS_HREG_RESERVED_0306:           *data = 0;                            break;
        case MDBS_HREG_RESERVED_0307:           *data = 0;                            break;
        case MDBS_HREG_RESERVED_0308:           *data = 0;                            break;
        case MDBS_HREG_RESERVED_0309:           *data = 0;                            break;
        case MDBS_HREG_RESERVED_030A:           *data = 0;                            break;
        case MDBS_HREG_RESERVED_030B:           *data = 0;                            break;
        case MDBS_HREG_RESERVED_030C:           *data = 0;                            break;
        case MDBS_HREG_RESERVED_030D:           *data = 0;                            break;
        case MDBS_HREG_RESERVED_030E:           *data = 0;                            break;
        case MDBS_HREG_RESERVED_030F:           *data = 0;                            break;

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
    mdbs_err_t  err     = MDBS_ERR_NONE;


    switch( idx )
    {
        case MDBS_HREG_V1_DEVICE_ID:
        case MDBS_HREG_V1_HARDWARE_ID:
        case MDBS_HREG_V1_RESERVED_02:
        case MDBS_HREG_V1_RESERVED_03:
        case MDBS_HREG_V1_FIRMWARE_ID_HI:
        case MDBS_HREG_V1_FIRMWARE_ID_LO:
        case MDBS_HREG_V1_RESERVED_06:
        case MDBS_HREG_V1_RESERVED_07:
        case MDBS_HREG_V1_SERIAL_NUM_00:
        case MDBS_HREG_V1_SERIAL_NUM_01:
        case MDBS_HREG_V1_SERIAL_NUM_02:
        case MDBS_HREG_V1_SERIAL_NUM_03:
        case MDBS_HREG_V1_SERIAL_NUM_04:
        case MDBS_HREG_V1_SERIAL_NUM_05:
        case MDBS_HREG_V1_SERIAL_NUM_06:
        case MDBS_HREG_V1_SERIAL_NUM_07:
            break;  //address exist, read-only register, do nothing

        case MDBS_HREG_V1_ERR_CODE_LAST:
            break;  //address exist, read-only register, do nothing

        case MDBS_HREG_V1_STARTS_COUNTER:
            dev.starts_cnt      = *data;
            break;

        case MDBS_HREG_V1_RESERVED_12:
        case MDBS_HREG_V1_RESERVED_13:
            //address exist, read-only register, do nothing
            break;

        case MDBS_HREG_V1_ADC_SCALE_mV:
        case MDBS_HREG_V1_ADC_RESOLUTION_BITS:
            //address exist, read-only register, do nothing
            break;

        case MDBS_HREG_V1_RESERVED_16:
        case MDBS_HREG_V1_RESERVED_17:
        case MDBS_HREG_V1_MCU_TEMP_CELS:
        case MDBS_HREG_V1_MCU_VDDA_mV:
        case MDBS_HREG_V1_RAW2PPM_FV_HI:
        case MDBS_HREG_V1_RAW2PPM_FV_LO:
        case MDBS_HREG_V1_RAW2PPM_FT_HI:
        case MDBS_HREG_V1_RAW2PPM_FT_LO:
        case MDBS_HREG_V1_RAW2PPM_FP_HI:
        case MDBS_HREG_V1_RAW2PPM_FP_LO:
            //address exist, read-only register, do nothing
            break;

        case MDBS_HREG_V1_SENS_OXGN_PPM_LO:
        case MDBS_HREG_V1_SENS_OXGN_PPM_HI:
        case MDBS_HREG_V1_SENS_TEMP_DIGC_HI:
        case MDBS_HREG_V1_SENS_TEMP_DIGC_LO:
        case MDBS_HREG_V1_SENS_PRES_HPA_HI:
        case MDBS_HREG_V1_SENS_PRES_HPA_LO:
        case MDBS_HREG_V1_RESERVED_26:
        case MDBS_HREG_V1_RESERVED_27:
            //address exist, read-only register, do nothing
            break;

        case MDBS_HREG_V1_SENS_RAW_HI:
        case MDBS_HREG_V1_SENS_RAW_LO:
        case MDBS_HREG_V1_TEMP_RAW_HI:
        case MDBS_HREG_V1_TEMP_RAW_LO:
        case MDBS_HREG_V1_PRES_RAW_HI:
        case MDBS_HREG_V1_PRES_RAW_LO:
            //address exist, read-only register, do nothing
            break;

        case MDBS_HREG_V1_SENS_MEAS_RMSE:
            //address exist, read-only register, do nothing
            break;

        case MDBS_HREG_V1_SENS_OFST_RAW:
            dev_afe_bias_set( &dev, *data );
            //nvm_write16( NVM_ADDR_SENS_OFST_RAW, (uint16_t *) data, 1 );
            break;


        case MDBS_HREG_V1_SENS_ZERO_TIMESTMP_HI:
            dev.sens.zero.timestmp.u16[ 1]  = *data;
            break;

        case MDBS_HREG_V1_SENS_ZERO_TIMESTMP_LO:
            dev.sens.zero.timestmp.u16[ 0]  = *data;
            break;

        case MDBS_HREG_V1_SENS_ZERO_OXGN_PPM_HI:
            dev.sens.zero.ppm.u16[ 1]       = *data;
            break;

        case MDBS_HREG_V1_SENS_ZERO_OXGN_PPM_LO:
            dev.sens.zero.ppm.u16[ 0]       = *data;
            dev.sens.zero.raw.i32           = dev.sens.meas.raw.i32;
            dev.sens.zero.temp_digc.f32     = dev.sens.temp.digc.f32;
            dev.sens.zero.pres_raw.i32      = dev.sens.pres.raw.i32;
            dev_sens_trim_restore( &dev.sens );
            dev_nvm_write( NVM_ADDR_SENS_ZERO_TIMESTMP_HI,  dev.sens.zero.timestmp.u16[ 1]   );
            dev_nvm_write( NVM_ADDR_SENS_ZERO_TIMESTMP_LO,  dev.sens.zero.timestmp.u16[ 0]   );
            dev_nvm_write( NVM_ADDR_SENS_ZERO_OXGN_PPM_HI,  dev.sens.zero.ppm.u16[ 1]   );
            dev_nvm_write( NVM_ADDR_SENS_ZERO_OXGN_PPM_LO,  dev.sens.zero.ppm.u16[ 0]   );
            dev_nvm_write( NVM_ADDR_SENS_ZERO_OXGN_RAW_HI,  dev.sens.zero.raw.u16[ 1]   );
            dev_nvm_write( NVM_ADDR_SENS_ZERO_OXGN_RAW_LO,  dev.sens.zero.raw.u16[ 0]   );
            dev_nvm_write( NVM_ADDR_SENS_ZERO_TEMP_DIGC_HI, dev.sens.zero.temp_digc.u16[ 1]  );
            dev_nvm_write( NVM_ADDR_SENS_ZERO_TEMP_DIGC_LO, dev.sens.zero.temp_digc.u16[ 0]  );
            dev_nvm_write( NVM_ADDR_SENS_ZERO_PRES_RAW_HI,  dev.sens.zero.pres_raw.u16[ 1]   );
            dev_nvm_write( NVM_ADDR_SENS_ZERO_PRES_RAW_LO,  dev.sens.zero.pres_raw.u16[ 0]   );
            break;

        case MDBS_HREG_V1_SENS_ZERO_OXGN_RAW_HI:
        case MDBS_HREG_V1_SENS_ZERO_OXGN_RAW_LO:
        case MDBS_HREG_V1_RESERVED_36:
        case MDBS_HREG_V1_RESERVED_37:
            //address exist, read-only register, do nothing
            break;

        case MDBS_HREG_V1_SENS_SPAN_TIMESTMP_HI:
            dev.sens.span.timestmp.u16[ 1]  = *data;
            break;

        case MDBS_HREG_V1_SENS_SPAN_TIMESTMP_LO:
            dev.sens.span.timestmp.u16[ 0]  = *data;
            break;

        case MDBS_HREG_V1_SENS_SPAN_OXGN_PPM_HI:
            dev.sens.span.ppm.u16[ 1]       = *data;
            break;

        case MDBS_HREG_V1_SENS_SPAN_OXGN_PPM_LO:
            dev.sens.span.ppm.u16[ 0]       = *data;
            dev.sens.span.raw.i32           = dev.sens.meas.raw.i32;
            dev.sens.span.temp_digc.f32     = dev.sens.temp.digc.f32;
            dev.sens.span.pres_raw.i32      = dev.sens.pres.raw.i32;
            dev_sens_trim_restore( &dev.sens);
            dev_nvm_write( NVM_ADDR_SENS_SPAN_TIMESTMP_HI,  dev.sens.span.timestmp.u16[ 1] );
            dev_nvm_write( NVM_ADDR_SENS_SPAN_TIMESTMP_LO,  dev.sens.span.timestmp.u16[ 0] );
            dev_nvm_write( NVM_ADDR_SENS_SPAN_OXGN_PPM_HI,  dev.sens.span.ppm.u16[ 1] );
            dev_nvm_write( NVM_ADDR_SENS_SPAN_OXGN_PPM_LO,  dev.sens.span.ppm.u16[ 0] );
            dev_nvm_write( NVM_ADDR_SENS_SPAN_OXGN_RAW_HI,  dev.sens.span.raw.u16[ 1] );
            dev_nvm_write( NVM_ADDR_SENS_SPAN_OXGN_RAW_LO,  dev.sens.span.raw.u16[ 0] );
            dev_nvm_write( NVM_ADDR_SENS_SPAN_TEMP_DIGC_HI, dev.sens.span.temp_digc.u16[ 1]);
            dev_nvm_write( NVM_ADDR_SENS_SPAN_TEMP_DIGC_LO, dev.sens.span.temp_digc.u16[ 0]);
            dev_nvm_write( NVM_ADDR_SENS_SPAN_PRES_RAW_HI,  dev.sens.span.pres_raw.u16[ 1] );
            dev_nvm_write( NVM_ADDR_SENS_SPAN_PRES_RAW_LO,  dev.sens.span.pres_raw.u16[ 0] );
            break;

        case MDBS_HREG_V1_SENS_SPAN_OXGN_RAW_HI:
        case MDBS_HREG_V1_SENS_SPAN_OXGN_RAW_LO:
        case MDBS_HREG_V1_RESERVED_3E:
        case MDBS_HREG_V1_RESERVED_3F:
            break;  //address exist, read-only register, do nothing


        //V2 CONF
        case MDBS_HREG_CONF_DEVICE_ID:
        case MDBS_HREG_CONF_HARDWARE_ID:
        case MDBS_HREG_CONF_RESERVED_0102:
        case MDBS_HREG_CONF_RESERVED_0103:
        case MDBS_HREG_CONF_FIRMWARE_ID_HI:
        case MDBS_HREG_CONF_FIRMWARE_ID_LO:
        case MDBS_HREG_RESERVED_0106:
        case MDBS_HREG_RESERVED_0107:
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
        case MDBS_HREG_RESERVED_0142:
        case MDBS_HREG_RESERVED_0143:
            break;  //address exist, read-only register, do nothing

        case MDBS_HREG_CONF_AFE_BIAS:
            dev_afe_bias_set( &dev, *data );
            //nvm_write16( NVM_ADDR_SENS_OFST_RAW, (uint16_t *) data, 1 );
            break;

        case MDBS_HREG_RESERVED_0145:
        case MDBS_HREG_RESERVED_0146:
        case MDBS_HREG_RESERVED_0147:
            break;  //address exist, read-only register, do nothing

        case MDBS_HREG_CONF_AFE_DRIFT_K_TEMP_HI:
            dev.sens.drift_k_temp.u16[ 1]               = *data;
            break;

        case MDBS_HREG_CONF_AFE_DRIFT_K_TEMP_LO:
            dev.sens.drift_k_temp.u16[ 0]               = *data;
            dev_nvm_write( NVM_ADDR_AFE_K_TEMP_DRIFT_HI,  dev.sens.drift_k_temp.u16[ 1] );
            dev_nvm_write( NVM_ADDR_AFE_K_TEMP_DRIFT_LO,  dev.sens.drift_k_temp.u16[ 0] );
            break;

        case MDBS_HREG_RESERVED_014A:
        case MDBS_HREG_RESERVED_014B:
            break;  //address exist, read-only register, do nothing

        case MDBS_HREG_CONF_AFE_DRIFT_K_PRES_HI:
            dev.sens.drift_k_pres.u16[ 0]               = *data;
            break;

        case MDBS_HREG_CONF_AFE_DRIFT_K_PRES_LO:
            dev.sens.drift_k_pres.u16[ 1]               = *data;
            dev_nvm_write( NVM_ADDR_AFE_K_PRES_DRIFT_HI,  dev.sens.drift_k_pres.u16[ 1] );
            dev_nvm_write( NVM_ADDR_AFE_K_PRES_DRIFT_LO,  dev.sens.drift_k_pres.u16[ 0] );
            break;

        case MDBS_HREG_RESERVED_014E:
        case MDBS_HREG_RESERVED_014F:
            break;  //address exist, read-only register, do nothing

        case MDBS_HREG_CONF_AD7799_MODE:
            dev_afe_adc_mode_set( &dev, *data );
            break;

        case MDBS_HREG_CONF_AD7799_CONF:
            dev_afe_adc_conf_set( &dev, *data );
            break;

        case MDBS_HREG_RESERVED_0152:
        case MDBS_HREG_RESERVED_0153:
            break;  //address exist, read-only register, do nothing

        case MDBS_HREG_CONF_AD7799_CHANNEL:
            dev_afe_adc_chnl_set( &dev, *data );
            break;

        case MDBS_HREG_CONF_AD7799_GAIN:
            dev_afe_adc_gain_set( &dev, *data );
            break;

        case MDBS_HREG_RESERVED_0156:
        case MDBS_HREG_RESERVED_0157:
        case MDBS_HREG_RESERVED_0158:
        case MDBS_HREG_RESERVED_0159:
        case MDBS_HREG_RESERVED_015A:
        case MDBS_HREG_RESERVED_015B:
        case MDBS_HREG_RESERVED_015C:
        case MDBS_HREG_RESERVED_015D:
        case MDBS_HREG_RESERVED_015E:
        case MDBS_HREG_RESERVED_015F:
            break;  //address exist, read-only register, do nothing

                //V2 MEAS
        case MDBS_HREG_SENS_MEAS_PPM_HI:
        case MDBS_HREG_SENS_MEAS_PPM_LO:
        case MDBS_HREG_SENS_TEMP_DIGC_HI:
        case MDBS_HREG_SENS_TEMP_DIGC_LO:
        case MDBS_HREG_SENS_PRES_HPA_HI:
        case MDBS_HREG_SENS_PRES_HPA_LO:
        case MDBS_HREG_SENS_RESERVED_0206:
        case MDBS_HREG_SENS_RESERVED_0207:
        case MDBS_HREG_SENS_MEAS_RMSE:
        case MDBS_HREG_SENS_MEAS_SENSOR_OFFSET_RAW:
        case MDBS_HREG_SENS_RESERVED_020A:
        case MDBS_HREG_SENS_RESERVED_020B:
        case MDBS_HREG_SENS_MCU_TEMP_DIGC:
        case MDBS_HREG_SENS_MCU_VDDA_mV:
        case MDBS_HREG_SENS_RESERVED_020E:
        case MDBS_HREG_SENS_RESERVED_020F:
        case MDBS_HREG_SENS_MEAS_ADC_RAW_HI:
        case MDBS_HREG_SENS_MEAS_ADC_RAW_LO:
        case MDBS_HREG_SENS_MEAS_TEMP_RAW_HI:
        case MDBS_HREG_SENS_MEAS_TEMP_RAW_LO:
        case MDBS_HREG_SENS_MEAS_PRES_RAW_HI:
        case MDBS_HREG_SENS_MEAS_PRES_RAW_LO:
        case MDBS_HREG_RESERVED_0216:
        case MDBS_HREG_RESERVED_0217:
        case MDBS_HREG_RESERVED_0218:
        case MDBS_HREG_RESERVED_0219:
        case MDBS_HREG_RESERVED_021A:
        case MDBS_HREG_RESERVED_021B:
        case MDBS_HREG_RESERVED_021C:
        case MDBS_HREG_RESERVED_021D:
        case MDBS_HREG_RESERVED_021E:
        case MDBS_HREG_RESERVED_021F:
            break;  //address exist, read-only register, do nothing

        case MDBS_HREG_SENS_LPF_FCUT:
            if( *data < CONF_SENS_LPF_FCUT_MAX )
            {
                dev.sens.lpf.fcut   = *data;
                dev_nvm_write( NVM_ADDR_SENS_LPF_FCUT,  dev.sens.lpf.fcut );
            }
            break;

        case MDBS_HREG_SENS_LPF_ORDER:
            if( *data < CONF_SENS_LPF_ORDER_MAX )
            {
                dev.sens.lpf.order  = *data;
                dev_nvm_write( NVM_ADDR_SENS_LPF_ORDER,  dev.sens.lpf.order );
            }
            break;

        case MDBS_HREG_RESERVED_0222:
        case MDBS_HREG_RESERVED_0223:
        case MDBS_HREG_RESERVED_0224:
        case MDBS_HREG_RESERVED_0225:
        case MDBS_HREG_RESERVED_0226:
        case MDBS_HREG_RESERVED_0227:
        case MDBS_HREG_RESERVED_0228:
        case MDBS_HREG_RESERVED_0229:
        case MDBS_HREG_RESERVED_022A:
        case MDBS_HREG_RESERVED_022B:
        case MDBS_HREG_RESERVED_022C:
        case MDBS_HREG_RESERVED_022D:
        case MDBS_HREG_RESERVED_022E:
        case MDBS_HREG_RESERVED_022F:
            break;  //address exist, read-only register, do nothing


                //V2 STS
        case MDBS_HREG_STS_LAST_ERROR:
            break;  //address exist, read-only register, do nothing

        case MDBS_HREG_STS_STARTS_COUNTER:
            dev.starts_cnt      = *data;
            break;

        case MDBS_HREG_RESERVED_0302:
        case MDBS_HREG_RESERVED_0303:
        case MDBS_HREG_RESERVED_0304:
        case MDBS_HREG_RESERVED_0305:
        case MDBS_HREG_RESERVED_0306:
        case MDBS_HREG_RESERVED_0307:
        case MDBS_HREG_RESERVED_0308:
        case MDBS_HREG_RESERVED_0309:
        case MDBS_HREG_RESERVED_030A:
        case MDBS_HREG_RESERVED_030B:
        case MDBS_HREG_RESERVED_030C:
        case MDBS_HREG_RESERVED_030D:
        case MDBS_HREG_RESERVED_030E:
        case MDBS_HREG_RESERVED_030F:
            break;  //address exist, read-only register, do nothing

        default:
            err     = MDBS_ERR_ILLEGAL_DATA_ADDRESS;
            break;
    }

    return( err );
}
