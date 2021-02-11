/**
  * @file    mdbs_dev.c
  * @brief   MODBUS Device implementation
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <string.h>
#include "mdbs.h"
#include "mdbs_dev_o2mb.h"
#include "sens.h"
#include "sys.h"
#include "app.h"


extern  sens_t          sens;


/**
  * @brief  
  * @param  None
  * @retval None
  */
mdbs_err_t
mdbs_coil_read(                         const   size_t                  idx,
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
mdbs_coil_write(                        const   size_t                  idx,
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
                case MDBS_HREG_DEVICE_ID_HI:
                        *data   = sens.mcu.device_id( 0 );
                        break;

                case MDBS_HREG_DEVICE_ID_LO:
                        *data   = sens.mcu.device_id( 1 );
                        break;

                case MDBS_HREG_HARDWARE_ID_HI:
                        //*data   = sens.mcu.hardware_id( 0 );
                        break;

                case MDBS_HREG_HARDWARE_ID_LO:
                        //*data   = sens.mcu.hardware_id( 1 );
                        break;

                case MDBS_HREG_FIRMWARE_ID_HI:
                        *data   = sens.mcu.firmware_id( 0 );
                        break;

                case MDBS_HREG_FIRMWARE_ID_MI:
                        *data   = sens.mcu.firmware_id( 1 );
                        break;

                case MDBS_HREG_FIRMWARE_ID_LO:
                        *data   = sens.mcu.firmware_id( 2 );
                        break;

                case MDBS_HREG_RESERVED_07:
                        *data   = 0;
                        break;

                case MDBS_HREG_SERIAL_NUM_00:
                        *data   = sens.mcu.serial_num( 0 );
                        break;

                case MDBS_HREG_SERIAL_NUM_01:
                        *data   = sens.mcu.serial_num( 1 );
                        break;

                case MDBS_HREG_SERIAL_NUM_02:
                        *data   = sens.mcu.serial_num( 2 );
                        break;

                case MDBS_HREG_SERIAL_NUM_03:
                        *data   = sens.mcu.serial_num( 3 );
                        break;

                case MDBS_HREG_SERIAL_NUM_04:
                        *data   = sens.mcu.serial_num( 4 );
                        break;

                case MDBS_HREG_SERIAL_NUM_05:
                        *data   = sens.mcu.serial_num( 5 );
                        break;

                case MDBS_HREG_SERIAL_NUM_06:
                        *data   = sens.mcu.serial_num( 6 );
                        break;

                case MDBS_HREG_SERIAL_NUM_07:
                        *data   = sens.mcu.serial_num( 7 );
                        break;


                case MDBS_HREG_ERR_CODE_LAST:
                        *data   = 0;
                        break;

                case MDBS_HREG_STARTS_COUNTER:
                        *data   = sens.mcu.starts_cnt;
                        break;

                case MDBS_HREG_RESERVED_12:
                case MDBS_HREG_RESERVED_13:
                case MDBS_HREG_RESERVED_14:
                case MDBS_HREG_RESERVED_15:
                case MDBS_HREG_RESERVED_16:
                case MDBS_HREG_RESERVED_17:
                        *data   = 0;
                        break;

                case MDBS_HREG_MCU_TEMP_CELS:
                        *data   = sens.mcu.degc;
                        break;

                case MDBS_HREG_MCU_VDDA_mV:
                        *data   = sens.mcu.vref_mV;
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
                        *data   = sens.oxgn.raw.u16[ 1];
                        break;

                case MDBS_HREG_SENS_RAW_LO:
                        *data   = sens.oxgn.raw.u16[ 0];
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
                case MDBS_HREG_DEVICE_ID_HI:
                case MDBS_HREG_DEVICE_ID_LO:
                case MDBS_HREG_HARDWARE_ID_HI:
                case MDBS_HREG_HARDWARE_ID_LO:
                case MDBS_HREG_FIRMWARE_ID_HI:
                case MDBS_HREG_FIRMWARE_ID_MI:
                case MDBS_HREG_FIRMWARE_ID_LO:
                case MDBS_HREG_RESERVED_07:
                case MDBS_HREG_SERIAL_NUM_00:
                case MDBS_HREG_SERIAL_NUM_01:
                case MDBS_HREG_SERIAL_NUM_02:
                case MDBS_HREG_SERIAL_NUM_03:
                case MDBS_HREG_SERIAL_NUM_04:
                case MDBS_HREG_SERIAL_NUM_05:
                case MDBS_HREG_SERIAL_NUM_06:
                case MDBS_HREG_SERIAL_NUM_07:
                        //address exist, read-only register, do nothing
                        break;

                case MDBS_HREG_ERR_CODE_LAST:
                        //address exist, read-only register, do nothing
                        break;

                case MDBS_HREG_STARTS_COUNTER:
                        sens.mcu.starts_cnt     = *data;
                        break;

                case MDBS_HREG_RESERVED_12:
                case MDBS_HREG_RESERVED_13:
                case MDBS_HREG_RESERVED_14:
                case MDBS_HREG_RESERVED_15:
                case MDBS_HREG_RESERVED_16:
                case MDBS_HREG_RESERVED_17:
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
                        sens.oxgn.offset                        = *data;
                        sys_nvm_write16( SYS_NVM_ADDR_SENS_OFST_RAW, &sens.oxgn.offset, 1 );
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
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P0_TIMESTMP_HI, &sens.trim.point[ 0].timestmp.u16[ 1], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P0_TIMESTMP_LO, &sens.trim.point[ 0].timestmp.u16[ 0], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P0_OXGN_PPM_HI, &sens.trim.point[ 0].oxgn_ppm.u16[ 1], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P0_OXGN_PPM_LO, &sens.trim.point[ 0].oxgn_ppm.u16[ 0], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P0_OXGN_RAW_HI, &sens.trim.point[ 0].oxgn_raw.u16[ 1], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P0_OXGN_RAW_LO, &sens.trim.point[ 0].oxgn_raw.u16[ 0], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P0_TEMP_DIGC_HI, &sens.trim.point[ 0].temp_digc.u16[ 1], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P0_TEMP_DIGC_LO, &sens.trim.point[ 0].temp_digc.u16[ 0], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P0_PRES_RAW_HI, &sens.trim.point[ 0].pres_raw.u16[ 1], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P0_PRES_RAW_LO, &sens.trim.point[ 0].pres_raw.u16[ 0], 1 );
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
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P1_TIMESTMP_HI, &sens.trim.point[ 1].timestmp.u16[ 1], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P1_TIMESTMP_LO, &sens.trim.point[ 1].timestmp.u16[ 0], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P1_OXGN_PPM_HI, &sens.trim.point[ 1].oxgn_ppm.u16[ 1], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P1_OXGN_PPM_LO, &sens.trim.point[ 1].oxgn_ppm.u16[ 0], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P1_OXGN_RAW_HI, &sens.trim.point[ 1].oxgn_raw.u16[ 1], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P1_OXGN_RAW_LO, &sens.trim.point[ 1].oxgn_raw.u16[ 0], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P1_TEMP_DIGC_HI, &sens.trim.point[ 1].temp_digc.u16[ 1], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P1_TEMP_DIGC_LO, &sens.trim.point[ 1].temp_digc.u16[ 0], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P1_PRES_RAW_HI, &sens.trim.point[ 1].pres_raw.u16[ 1], 1 );
                        sys_nvm_write16( SYS_NVM_ADDR_TRIM_P1_PRES_RAW_LO, &sens.trim.point[ 1].pres_raw.u16[ 0], 1 );

                        break;

                case MDBS_HREG_SENS_TRIM_P1_OXGN_RAW_HI:
                case MDBS_HREG_SENS_TRIM_P1_OXGN_RAW_LO:
                case MDBS_HREG_RESERVED_3E:
                case MDBS_HREG_RESERVED_3F:
                        //address exist, read-only register, do nothing
                        break;


                default:
                        err     = MDBS_ERR_ILLEGAL_DATA_ADDRESS;
                        break;
        }

        return( err );
}
