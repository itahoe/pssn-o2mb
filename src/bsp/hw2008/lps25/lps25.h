/**
  * @file    lps25.h
  * @brief   LPS25 driver header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef LPS25_H
#define LPS25_H


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


//#define LPS25_DEV_ADDR                  0x5C
#define LPS25_ID                        0xBD


typedef enum    lps25_reg_e
{
        LPS25_REG_REF_P_XL              = 0x08, //RW, 
        LPS25_REG_REF_P_L               = 0x09, //RW, 
        LPS25_REG_REF_P_H               = 0x0A, //RW, 
        LPS25_REG_WHO_AM_I              = 0x0F, //RW, 
        LPS25_REG_RES_CONF              = 0x10, //RW, 
        LPS25_REG_CTRL_REG1             = 0x20, //RW, 
        LPS25_REG_CTRL_REG2             = 0x21, //RW, 
        LPS25_REG_CTRL_REG3             = 0x22, //RW, 
        LPS25_REG_CTRL_REG4             = 0x23, //RW, 
        LPS25_REG_INTERRUPT_CFG         = 0x24, //RW, 
        LPS25_REG_INT_SOURCE            = 0x25, //RW, 
        LPS25_REG_STATUS_REG            = 0x27, //RW, 
        LPS25_REG_PRESS_OUT_XL          = 0x28, //RW, 
        LPS25_REG_PRESS_OUT_L           = 0x29, //RW, 
        LPS25_REG_PRESS_OUT_H           = 0x2A, //RW, 
        LPS25_REG_TEMP_OUT_L            = 0x2B, //RW, 
        LPS25_REG_TEMP_OUT_H            = 0x2C, //RW, 
        LPS25_REG_FIFO_CTRL             = 0x2E, //RW, 
        LPS25_REG_FIFO_STATUS           = 0x2F, //RW, 
        LPS25_REG_THS_P_L               = 0x30, //RW, 
        LPS25_REG_THS_P_H               = 0x31, //RW, 
        LPS25_REG_RPDS_L                = 0x39, //RW, 
        LPS25_REG_RPDS_H                = 0x3A, //RW, 
} lps25_reg_t;


typedef enum    pls25_rate_e
{
        LPS25_RATE_ONE_SHOT             = 0x0,
        LPS25_RATE_1_Hz                 = 0x1,
        LPS25_RATE_7_Hz                 = 0x2,
        LPS25_RATE_12_5_Hz              = 0x3,
        LPS25_RATE_25_Hz                = 0x4,
} pls25_rate_t;


typedef enum    pls25_ctl_e
{
        LPS25_CTL_POWER_DOWN,
} pls25_ctl_t;


/*******************************************************************************
*
*******************************************************************************/
float           lps25_pressure_raw_to_hpa( const int32_t raw );
float           lps25_pressure_raw_to_mbar( const int32_t raw );
float           lps25_temperature_raw_to_digc( const int16_t raw );

bool            lps25_init( void );
uint8_t         lps25_get_id( void );
void            lps25_reset( void );
uint8_t         lps25_get_data_rate( void );
void            lps25_set_data_rate( const pls25_rate_t rate );
int32_t         lps25_get_pressure_raw( void );
int16_t         lps25_get_temperature_raw( void );
uint16_t        lps25_get_pressure_offset_raw( void );
void            lps25_set_pressure_offset_raw( const uint16_t raw );

uint8_t
lps25_get_ctl(                          const   pls25_ctl_t     ctl );

void
lps25_set_ctl(                          const   pls25_ctl_t     ctl,
                                        const   uint8_t         d );


/*******************************************************************************
* LPS25_X
*******************************************************************************/
void    lps25_x_init( void );
void    lps25_x_read( const uint8_t, uint8_t *, size_t );
void    lps25_x_write( const uint8_t, const uint8_t *, uint8_t );


#endif	// LPS25_H
