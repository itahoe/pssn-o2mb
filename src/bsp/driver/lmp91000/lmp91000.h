/**
  * @file    lmp91000.h
  * @brief   LMP91000 driver header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef LMP91000_H
#define LMP91000_H


#include <stdint.h>


/*******************************************************************************
* PUBLIC CONSTANTS
*******************************************************************************/
#define LMP91000_REG_STATUS             0x00 //RO
#define LMP91000_REG_LOCK               0x01 //RW
#define LMP91000_REG_TIACN              0x10 //RW
#define LMP91000_REG_REFCN              0x11 //RW
#define LMP91000_REG_MODECN             0x12 //RW


/**
  * 7.6 Registers Maps
  * If writing to a reserved bit, user must write only 0.
  * Readback value is unspecified and should be discarded.
  */


/*******************************************************************************
* STATUS -- Status Register (Address 0x00)
*******************************************************************************/
typedef enum    lmp91000_status_e
{
        LMP91000_STATUS_NOT_READY               = 0x0,
        LMP91000_STATUS_READY                   = 0x1,
        LMP91000_STATUS_MASK                    = 0x1,
} lmp91000_status_t;


/*******************************************************************************
* LOCK -- Protection Register (Address 0x01)
*******************************************************************************/
/*
typedef enum    lmp91000_lock_e
{
        LMP91000_LOCK_WRITE_ENABLE              = 0x0,
        LMP91000_LOCK_WRITE_DISABLE             = 0x1,
        LMP91000_LOCK_MASK                      = 0x1,
} lmp91000_lock_t;
*/

/*******************************************************************************
* TIACN -- TIA Control Register (Address 0x10)
*******************************************************************************/
typedef enum    lmp91000_tia_gain_e
{
        LMP91000_TIA_GAIN_R_EXT                 = 0x0,
        LMP91000_TIA_GAIN_2K75                  = 0x1,
        LMP91000_TIA_GAIN_3K5                   = 0x2,
        LMP91000_TIA_GAIN_7K                    = 0x3,
        LMP91000_TIA_GAIN_14K                   = 0x4,
        LMP91000_TIA_GAIN_35K                   = 0x5,
        LMP91000_TIA_GAIN_120K                  = 0x6,
        LMP91000_TIA_GAIN_350K                  = 0x7,
        LMP91000_TIA_GAIN_MASK                  = 0x7,
} lmp91000_tia_gain_t;


typedef enum    lmp91000_rload_e
{
        LMP91000_RLOAD_10R                      = 0x0,
        LMP91000_RLOAD_33R                      = 0x1,
        LMP91000_RLOAD_50R                      = 0x2,
        LMP91000_RLOAD_100R                     = 0x3,
        LMP91000_RLOAD_MASK                     = 0x3,
} lmp91000_rload_t;


/*******************************************************************************
* REFCN -- Reference Control Register (Address 0x11)
*******************************************************************************/
typedef enum    lmp91000_ref_src_e
{
        LMP91000_REF_SRC_INTERNAL               = 0x0,
        LMP91000_REF_SRC_EXTERNAL               = 0x1,
        LMP91000_REF_SRC_MASK                   = 0x1,
} lmp91000_ref_src_t;


typedef enum    lmp91000_int_zero_e
{
        LMP91000_INT_ZERO_20_PERCENT            = 0x0,
        LMP91000_INT_ZERO_50_PERCENT            = 0x1,
        LMP91000_INT_ZERO_67_PERCENT            = 0x2,
        LMP91000_INT_ZERO_BYPASS                = 0x3,
        LMP91000_INT_ZERO_MASK                  = 0x3,
} lmp91000_int_zero_t;


typedef enum    lmp91000_bias_sign_e
{
        LMP91000_BIAS_SIGN_NEGATIVE             = 0x0,
        LMP91000_BIAS_SIGN_POSITIVE             = 0x1,
        LMP91000_BIAS_SIGN_MASK                 = 0x1,
} lmp91000_bias_sign_t;


typedef enum    lmp91000_bias_e
{
        LMP91000_BIAS_0_PCT                     = 0x0,
        LMP91000_BIAS_1_PCT                     = 0x1,
        LMP91000_BIAS_2_PCT                     = 0x2,
        LMP91000_BIAS_4_PCT                     = 0x3,
        LMP91000_BIAS_6_PCT                     = 0x4,
        LMP91000_BIAS_8_PCT                     = 0x5,
        LMP91000_BIAS_10_PCT                    = 0x6,
        LMP91000_BIAS_12_PCT                    = 0x7,
        LMP91000_BIAS_14_PCT                    = 0x8,
        LMP91000_BIAS_16_PCT                    = 0x9,
        LMP91000_BIAS_18_PCT                    = 0xA,
        LMP91000_BIAS_20_PCT                    = 0xB,
        LMP91000_BIAS_22_PCT                    = 0xC,
        LMP91000_BIAS_24_PCT                    = 0xD,
        LMP91000_BIAS_MASK                      = 0xF,
} lmp91000_bias_t;


/*******************************************************************************
* MODECN -- Mode Control Register (Address 0x12)
*******************************************************************************/
typedef enum    lmp91000_fet_short_e
{
        LMP91000_FET_SHORT_DISABLE              = 0x0,
        LMP91000_FET_SHORT_ENABLE               = 0x1,
        LMP91000_FET_SHORT_MASK                 = 0x1,
} lmp91000_fet_short_t;


typedef enum    lmp91000_mode_e
{
        LMP91000_MODE_DEEP_SLEEP                = 0x0,
        LMP91000_MODE_2_LEAD_CELL               = 0x1,
        LMP91000_MODE_STANDBY                   = 0x2,
        LMP91000_MODE_3_LEAD_CELL               = 0x3,
        LMP91000_MODE_TEMP_MEAS_TIA_OFF         = 0x6,
        LMP91000_MODE_TEMP_MEAS_TIA_ON          = 0x7,
        LMP91000_MODE_MASK                      = 0x7,
} lmp91000_mode_t;


/*******************************************************************************
* FUNCTION PROTOTYPES
*******************************************************************************/
void    lmp91000_write_lock( void );
void    lmp91000_write_unlock( void );
uint8_t lmp91000_write_status( void );

void    lmp91000_set_tia_gain(          const   lmp91000_tia_gain_t     );
void    lmp91000_set_rload(             const   lmp91000_rload_t        );
void    lmp91000_set_reference_source(  const   lmp91000_ref_src_t      );
void    lmp91000_set_internal_zero(     const   lmp91000_int_zero_t     );
void    lmp91000_set_bias_sign(         const   lmp91000_bias_sign_t    );
void    lmp91000_set_bias(              const   lmp91000_bias_t         );
void    lmp91000_set_fet_short(         const   lmp91000_fet_short_t    );
void    lmp91000_set_mode(              const   lmp91000_mode_t         );

lmp91000_tia_gain_t     lmp91000_get_tia_gain( void );
lmp91000_rload_t        lmp91000_get_rload( void );
lmp91000_ref_src_t      lmp91000_get_reference_source( void );
lmp91000_int_zero_t     lmp91000_get_internal_zero( void );
lmp91000_bias_sign_t    lmp91000_get_bias_sign( void );
lmp91000_bias_t         lmp91000_get_bias( void );
lmp91000_fet_short_t    lmp91000_get_fet_short( void );
lmp91000_mode_t         lmp91000_get_mode( void );

float   lmp91000_temperature_mV_to_digc( const float );
float   lmp91000_temperature_raw24_to_digc( const int32_t );


uint8_t lmp91000_x_read( const uint8_t );
void    lmp91000_x_write( const uint8_t, uint8_t );


#endif	// LMP91000_H
