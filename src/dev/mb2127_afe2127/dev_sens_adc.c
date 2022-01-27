/**
  * @file    dev_sens_adc.c
  * @brief   Device Sensor ADC
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <string.h>
#include "dev.h"
#include "stm32.h"
#include "nvm.h"
#include "ad7799.h"
#include "sma.h"



#define CONF_DEV_ADC_GAIN_THLD_HI            ((AD7799_SPAN / 8) * 3)
#define CONF_DEV_ADC_GAIN_THLD_LO            ((AD7799_SPAN / 8) * 1)



/**
  * @brief  
  * @param  None
  * @retval None
  */


/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
dev_afe_bias_get(                       const   dev_t *         p )
{
        return( p->afe.bias );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void
dev_afe_bias_set(                               dev_t *         p,
                                        const   uint16_t        data )
{
        p->afe.bias     = data;
        stm32_tim3_ch4_update( p->afe.bias );
        dev_nvm_write( NVM_ADDR_AFE_BIAS, p->afe.bias );
}

/*******************************************************************************
* AFE ADC
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
dev_afe_adc_mode_get(                   const   dev_t *         p )
{
        return( p->ad7799.reg.mode.u16 );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void
dev_afe_adc_mode_set(                           dev_t *         p,
                                        const   uint16_t        val )
{
        p->ad7799.reg.mode.u16         = val;
        ad7799_write( AD7799_REG_MODE, p->ad7799.reg.mode.u16 );
        dev_nvm_write( NVM_ADDR_AFE_ADC_REG_MODE, p->ad7799.reg.mode.u16 );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
dev_afe_adc_conf_get(                   const   dev_t *         p )
{
        return( p->ad7799.reg.conf.u16 );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void
dev_afe_adc_conf_set(                           dev_t *         p,
                                        const   uint16_t        val )
{
        p->ad7799.reg.conf.u16         = val;
        ad7799_write( AD7799_REG_CONF, p->ad7799.reg.conf.u16 );
        dev_nvm_write( NVM_ADDR_AFE_ADC_REG_CONF, p->ad7799.reg.conf.u16 );
}

/*******************************************************************************
*
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
int
dev_afe_adc_psw_get(                            dev_t *         p )
{
        return( p->ad7799.reg.mode.psw );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void
dev_afe_adc_psw_set(                            dev_t *         p,
                                        const   int             val )
{
        p->ad7799.reg.mode.psw         = val ?  1 : 0;
        ad7799_write( AD7799_REG_MODE, p->ad7799.reg.mode.u16 );
        dev_nvm_write( NVM_ADDR_AFE_ADC_REG_MODE, p->ad7799.reg.mode.u16 );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
int
dev_afe_adc_unipolar_get(               const   dev_t *         p )
{
        return( p->ad7799.reg.conf.unipolar );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void
dev_afe_adc_unipolar_set(                       dev_t *         p,
                                        const   int             val )
{
        p->ad7799.reg.conf.unipolar    = val ?  1 : 0;
        ad7799_write( AD7799_REG_CONF, p->ad7799.reg.conf.u16 );
        dev_nvm_write( NVM_ADDR_AFE_ADC_REG_CONF, p->ad7799.reg.conf.u16 );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
int
dev_afe_adc_buffer_enable_get(          const   dev_t *         p )
{
        return( p->ad7799.reg.conf.buf_en );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void
dev_afe_adc_buffer_enable_set(                  dev_t *         p,
                                        const   int             val )
{
        p->ad7799.reg.conf.buf_en      = val ?  1 : 0;
        ad7799_write( AD7799_REG_CONF, p->ad7799.reg.conf.u16 );
        dev_nvm_write( NVM_ADDR_AFE_ADC_REG_CONF, p->ad7799.reg.conf.u16 );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
dev_afe_adc_chnl_get(                   const   dev_t *         p )
{
        return( p->ad7799.reg.conf.chnl );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void
dev_afe_adc_chnl_set(                           dev_t *         p,
                                        const   uint16_t        channel )
{
        p->ad7799.reg.conf.chnl        = (ad7799_chnl_t) channel;
        ad7799_write( AD7799_REG_CONF, p->ad7799.reg.conf.u16 );
        dev_nvm_write( NVM_ADDR_AFE_ADC_REG_CONF, p->ad7799.reg.conf.u16 );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
dev_afe_adc_gain_get(                   const   dev_t *         p )
{
        return( p->ad7799.reg.conf.gain );
       
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void
dev_afe_adc_gain_set(                           dev_t *         p,
                                        const   uint16_t        gain )
{
        p->ad7799.reg.conf.gain        = (ad7799_gain_t) gain;
        ad7799_write( AD7799_REG_CONF, p->ad7799.reg.conf.u16 );
        dev_nvm_write( NVM_ADDR_AFE_ADC_REG_CONF, p->ad7799.reg.conf.u16 );
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
static
void
dev_afe_adc_gain_inc(                           dev_t *         p )
{
        if( p->ad7799.reg.conf.gain < AD7799_GAIN_MAX )
        {
                p->ad7799.reg.conf.gain++;
                ad7799_write( AD7799_REG_CONF, p->ad7799.reg.conf.u16 );
                //printf( "gain: %d\n", p->ad7799.reg.conf.gain );
        }
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
static
void
dev_afe_adc_gain_dec(                           dev_t *         p )
{
        if( p->ad7799.reg.conf.gain > 0 )
        {
                p->ad7799.reg.conf.gain--;
                ad7799_write( AD7799_REG_CONF, p->ad7799.reg.conf.u16 );
                //printf( "gain: %d\n", p->ad7799.reg.conf.gain );
        }
}
