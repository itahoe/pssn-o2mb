/**
  * @file    mdbs_dev.h
  * @brief   MODBUS Device
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef MDBS_DEV_H
#define MDBS_DEV_H


typedef enum    mdbs_hreg_e
{
        MDBS_HREG_DEVICE_ID_HI          = 0,
        MDBS_HREG_DEVICE_ID_LO,
        MDBS_HREG_HARDWARE_ID_HI,
        MDBS_HREG_HARDWARE_ID_LO,
        MDBS_HREG_FIRMWARE_ID_HI,
        MDBS_HREG_FIRMWARE_ID_MI,
        MDBS_HREG_FIRMWARE_ID_LO,
        MDBS_HREG_RESERVED_07,
        MDBS_HREG_SERIAL_NUM_00,
        MDBS_HREG_SERIAL_NUM_01,
        MDBS_HREG_SERIAL_NUM_02,
        MDBS_HREG_SERIAL_NUM_03,
        MDBS_HREG_SERIAL_NUM_04,
        MDBS_HREG_SERIAL_NUM_05,
        MDBS_HREG_SERIAL_NUM_06,
        MDBS_HREG_SERIAL_NUM_07,

        MDBS_HREG_ERR_CODE_LAST,                //= 16,
        MDBS_HREG_STARTS_COUNTER,
        MDBS_HREG_RESERVED_12,
        MDBS_HREG_RESERVED_13,
        MDBS_HREG_RESERVED_14,
        MDBS_HREG_RESERVED_15,
        MDBS_HREG_RESERVED_16,
        MDBS_HREG_RESERVED_17,
        MDBS_HREG_MCU_TEMP_CELS,
        MDBS_HREG_MCU_VDDA_mV,
        MDBS_HREG_RESERVED_1A,
        MDBS_HREG_RESERVED_1B,
        MDBS_HREG_RESERVED_1C,
        MDBS_HREG_RESERVED_1D,
        MDBS_HREG_RESERVED_1E,
        MDBS_HREG_RESERVED_1F,

        MDBS_HREG_SENS_OXGN_PPM_HI,             //= 32,
        MDBS_HREG_SENS_OXGN_PPM_LO,
        MDBS_HREG_SENS_TEMP_DIGC_HI,
        MDBS_HREG_SENS_TEMP_DIGC_LO,
        MDBS_HREG_SENS_PRES_HPA_HI,
        MDBS_HREG_SENS_PRES_HPA_LO,
        MDBS_HREG_RESERVED_26,
        MDBS_HREG_RESERVED_27,
        MDBS_HREG_SENS_RAW_HI,
        MDBS_HREG_SENS_RAW_LO,
        MDBS_HREG_TEMP_RAW_HI,
        MDBS_HREG_TEMP_RAW_LO,
        MDBS_HREG_PRES_RAW_HI,
        MDBS_HREG_PRES_RAW_LO,
        MDBS_HREG_SENS_SLOPE_RAW,
        MDBS_HREG_SENS_OFST_RAW,

        MDBS_HREG_SENS_TRIM_P0_TIMESTMP_HI,     //= 48,
        MDBS_HREG_SENS_TRIM_P0_TIMESTMP_LO,
        MDBS_HREG_SENS_TRIM_P0_OXGN_PPM_HI,
        MDBS_HREG_SENS_TRIM_P0_OXGN_PPM_LO,
        MDBS_HREG_RESERVED_34,
        MDBS_HREG_RESERVED_35,
        MDBS_HREG_RESERVED_36,
        MDBS_HREG_RESERVED_37,
        MDBS_HREG_SENS_TRIM_P1_TIMESTMP_HI,
        MDBS_HREG_SENS_TRIM_P1_TIMESTMP_LO,
        MDBS_HREG_SENS_TRIM_P1_OXGN_PPM_HI,
        MDBS_HREG_SENS_TRIM_P1_OXGN_PPM_LO,
        MDBS_HREG_RESERVED_3C,
        MDBS_HREG_RESERVED_3D,
        MDBS_HREG_RESERVED_3E,
        MDBS_HREG_RESERVED_3F,


        MDBS_HREG_MAX,
} mdbs_hreg_t;


#endif  //MDBS_DEV_H
