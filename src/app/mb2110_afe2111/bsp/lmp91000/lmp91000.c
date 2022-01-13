/**
  * @file    lmp91000.c
  * @brief   LMP91000 driver
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "lmp91000.h"


/*******************************************************************************
* Status of Device
*******************************************************************************/
/**
  * @brief  None
  * @param  None
  * @retval None
  */
lmp91000_status_t
lmp91000_get_status( void )
{
        lmp91000_status_t       status;
        uint8_t                 reg;


        reg     = lmp91000_x_read( LMP91000_REG_STATUS );
        status  = (lmp91000_status_t) ((reg >> 0) & LMP91000_STATUS_MASK);

        return( status );
}


/*******************************************************************************
* Write protection
*******************************************************************************/
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void
lmp91000_write_lock( void )
{
        lmp91000_x_write( LMP91000_REG_LOCK, 0x01 );
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
void
lmp91000_write_unlock( void )
{
        lmp91000_x_write( LMP91000_REG_LOCK, 0x00 );
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
uint8_t 
lmp91000_write_status( void )
{
        return( lmp91000_x_read( LMP91000_REG_TIACN ) & 0x01 );
}


/*******************************************************************************
* TIA feedback resistance selection
*******************************************************************************/
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void
lmp91000_set_tia_gain(                  const   lmp91000_tia_gain_t     gain )
{
        uint8_t                 reg;


        reg     = lmp91000_x_read( LMP91000_REG_TIACN );
        reg     &= ~( LMP91000_TIA_GAIN_MASK << 2);
        reg     |= (gain << 2);
        lmp91000_x_write( LMP91000_REG_TIACN, reg );
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
lmp91000_tia_gain_t
lmp91000_get_tia_gain( void )
{
        lmp91000_tia_gain_t     gain;
        uint8_t                 reg;


        reg     = lmp91000_x_read( LMP91000_REG_TIACN );
        gain    = (lmp91000_tia_gain_t) ((reg >> 2) & LMP91000_TIA_GAIN_MASK);

        return( gain );
}


/*******************************************************************************
* RLoad selection
*******************************************************************************/
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void
lmp91000_set_rload(                     const   lmp91000_rload_t        rload )
{
        uint8_t                 reg;


        reg     = lmp91000_x_read( LMP91000_REG_TIACN );
        reg     &= ~( LMP91000_RLOAD_MASK << 0);
        reg     |= (rload << 0);
        lmp91000_x_write( LMP91000_REG_TIACN, reg );
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
lmp91000_rload_t
lmp91000_get_rload( void )
{
        lmp91000_rload_t        rload;
        uint8_t                 reg;


        reg     = lmp91000_x_read( LMP91000_REG_TIACN );
        rload   = (lmp91000_rload_t) ((reg >> 0) & LMP91000_RLOAD_MASK);

        return( rload );
}


/*******************************************************************************
* Reference voltage source selection
*******************************************************************************/
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void
lmp91000_set_reference_source(          const   lmp91000_ref_src_t      ref )
{
        uint8_t                 reg;


        reg     = lmp91000_x_read( LMP91000_REG_REFCN );
        reg     &= ~( LMP91000_REF_SRC_MASK << 7);
        reg     |= (ref << 7);
        lmp91000_x_write( LMP91000_REG_REFCN, reg );
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
lmp91000_ref_src_t
lmp91000_get_reference_source( void )
{
        lmp91000_ref_src_t      ref;
        uint8_t                 reg;


        reg     = lmp91000_x_read( LMP91000_REG_REFCN );
        ref     = (lmp91000_ref_src_t) ((reg >> 7) & LMP91000_REF_SRC_MASK);

        return( ref );
}


/*******************************************************************************
* Internal zero selection (Percentage of the source reference)
*******************************************************************************/
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void
lmp91000_set_internal_zero(             const   lmp91000_int_zero_t     zero )
{
        uint8_t                 reg;


        reg     = lmp91000_x_read( LMP91000_REG_REFCN );
        reg     &= ~( LMP91000_INT_ZERO_MASK << 5);
        reg     |= (zero << 5);
        lmp91000_x_write( LMP91000_REG_REFCN, reg );
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
lmp91000_int_zero_t
lmp91000_get_internal_zero( void )
{
        lmp91000_int_zero_t     zero;
        uint8_t                 reg;


        reg     = lmp91000_x_read( LMP91000_REG_REFCN );
        zero    = (lmp91000_int_zero_t) ((reg >> 5) & LMP91000_INT_ZERO_MASK);

        return( zero );
}


/*******************************************************************************
* Selection of the Bias polarity
*******************************************************************************/
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void
lmp91000_set_bias_sign(                 const   lmp91000_bias_sign_t    sign )
{
        uint8_t                 reg;


        reg     = lmp91000_x_read( LMP91000_REG_REFCN );
        reg     &= ~( LMP91000_BIAS_SIGN_MASK << 4);
        reg     |= (sign << 4);
        lmp91000_x_write( LMP91000_REG_REFCN, reg );
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
lmp91000_bias_sign_t
lmp91000_get_bias_sign( void )
{
        lmp91000_bias_sign_t    sign;
        uint8_t                 reg;


        reg     = lmp91000_x_read( LMP91000_REG_REFCN );
        sign    = (lmp91000_bias_sign_t) ((reg >> 4) & LMP91000_BIAS_SIGN_MASK);

        return( sign );
}


/*******************************************************************************
* BIAS selection (Percentage of the source reference)
*******************************************************************************/
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void
lmp91000_set_bias(                      const   lmp91000_bias_t         mode )
{
        uint8_t         reg;


        reg     = lmp91000_x_read( LMP91000_REG_REFCN );
        reg     &= ~( LMP91000_BIAS_MASK << 0);
        reg     |= (mode << 0);
        lmp91000_x_write( LMP91000_REG_REFCN, reg );
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
lmp91000_bias_t
lmp91000_get_bias( void )
{
        lmp91000_bias_t         bias;
        uint8_t                 reg;


        reg     = lmp91000_x_read( LMP91000_REG_REFCN );
        bias    = (lmp91000_bias_t) ((reg >> 0) & LMP91000_BIAS_MASK);

        return( bias );
}


/*******************************************************************************
* Shorting FET feature
*******************************************************************************/
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void
lmp91000_set_fet_short(                 const   lmp91000_fet_short_t    shrt )
{
        uint8_t         reg;


        reg     = lmp91000_x_read( LMP91000_REG_MODECN );
        reg     &= ~(LMP91000_FET_SHORT_MASK << 7);
        reg     |= (shrt << 7);
        lmp91000_x_write( LMP91000_REG_MODECN, reg );
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
lmp91000_fet_short_t
lmp91000_get_fet_short( void )
{
        uint8_t         reg     = lmp91000_x_read( LMP91000_REG_MODECN );

        return( (reg & 0x80) ? LMP91000_FET_SHORT_ENABLE : LMP91000_FET_SHORT_DISABLE );
}


/*******************************************************************************
* Mode of Operation selection
*******************************************************************************/
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void
lmp91000_set_mode(                      const   lmp91000_mode_t         mode )
{
        uint8_t         reg;


        reg     = lmp91000_x_read( LMP91000_REG_MODECN );
        reg     &= ~( LMP91000_MODE_MASK );
        reg     |= mode;
        lmp91000_x_write( LMP91000_REG_MODECN, reg );
}


/**
  * @brief  None
  * @param  None
  * @retval None
  */
lmp91000_mode_t
lmp91000_get_mode( void )
{
        lmp91000_mode_t         mode;
        uint8_t                 reg     = lmp91000_x_read( LMP91000_REG_MODECN );


        mode    =  (lmp91000_mode_t) ((reg >> 0) & LMP91000_MODE_MASK);

        return( mode );
}



/*******************************************************************************
* Conversions
*******************************************************************************/
/*
#define TEMP_EQN_OFFSET 1562.2                                                 // LMP91000 DS pg 12: V=(-8.16mV/°C)*T+1562.2mV
#define TEMP_EQN_SLOPE  8.16                                                   // Linear approximation over temp range 20C to 50C
#define ADC12_RATIO     0.61035                                                // 2500/4096 (2.5V reference & 12bit converter)
#define ADC10_RATIO     1.46484375                                             // 1500/1024 (1.5V reference & 10bit converter)
*/

/**
  * @brief  Convert temperature from mV to degrees Celsius
  * @param  None
  * @retval None
  */
float
lmp91000_temperature_mV_to_digc(                const   float           t_mV )
{
      //measured_temp = (TEMP_EQN_OFFSET - vout) / TEMP_EQN_SLOPE;               
                                                                               // LMP91000 DS pg12: V=(-8.16mV/°C)*T+1562.2mV
                                                                               // Linear approximation over temp range 20C to 50C
        //float           adc_lsb_mV      = ADC_VREF_mV / ((uint32_t) 1 << 24);
        float           t_offset        = 1562.2;
        float           t_slope         = 8.16;
        float           t_digc          = (t_offset - t_mV) / t_slope;


        return( t_digc );
}


/**
  * @brief  Convert temperature from RAW 24bit to degrees Celsius
  * @param  None
  * @retval None
  */
float
lmp91000_temperature_raw24_to_digc(             const   int32_t         t_raw )
{
/*
      vout = sum_adc_data * ADC12_RATIO;                                       // LMP91000 vout
      measured_temp = (TEMP_EQN_OFFSET - vout) / TEMP_EQN_SLOPE;               
                                                                               // LMP91000 DS pg12: V=(-8.16mV/°C)*T+1562.2mV
                                                                               // Linear approximation over temp range 20C to 50C
*/

/*
      vout = sum_adc_data * ADC12_RATIO;                                       // LMP91000 vout
      measured_temp = (TEMP_EQN_OFFSET - vout) / TEMP_EQN_SLOPE;               
                                                                               // LMP91000 DS pg12: V=(-8.16mV/°C)*T+1562.2mV
                                                                               // Linear approximation over temp range 20C to 50C
*/
        float           t_digc  = 0;


        return( t_digc );
}
