/**
  * @file    dev.c
  * @brief   Device implementation
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <string.h>
#include "dev.h"
#include "nvm.h"


/**
  * @brief  
  * @param  None
  * @retval None
  */
int
dev_init(                               const   dev_t *         p )
{

        return( 0 );
}


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
/*
        lmp91000_set_bias( (lmp91000_bias_t) (offset & LMP91000_BIAS_MASK) );
        sens.oxgn.offset          = offset;
        nvm_write16( NVM_ADDR_SENS_OFST_RAW, (uint16_t *) &offset, 1 );

        return( sens.oxgn.offset );
*/
/*
        p->afe.bias_raw         = data;
        stm32_tim3_ch4_update( p->afe.bias_raw );
        nvm_write16( NVM_ADDR_SENS_OFST_RAW, &p->afe.bias_raw, 1 );
*/
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
        ad7799_reg_mode_write( p->ad7799.reg.mode );
        nvm_write16( NVM_ADDR_AFE_ADC_REG_MODE, &p->ad7799.reg.mode.u16, 1 );
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
        ad7799_reg_conf_write( p->ad7799.reg.conf );
        nvm_write16( NVM_ADDR_AFE_ADC_REG_CONF, &p->ad7799.reg.conf.u16, 1 );
}


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
        //ad7799_set_pwr_swtch( dev.mcu.spi.ad7799.reg.mode.psw ? true : false );
        ad7799_reg_mode_write( p->ad7799.reg.mode );
        nvm_write16( NVM_ADDR_AFE_ADC_REG_MODE, &p->ad7799.reg.mode.u16, 1 );
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
        ad7799_reg_conf_write( p->ad7799.reg.conf );
        nvm_write16( NVM_ADDR_AFE_ADC_REG_CONF, (uint16_t *) &p->ad7799.reg.conf.u16, 1 );
}


int
dev_afe_adc_buffer_enable_get(          const   dev_t *         p )
{
        return( p->ad7799.reg.conf.buf_en );
}


void
dev_afe_adc_buffer_enable_set(                  dev_t *         p,
                                        const   int             val )
{
        p->ad7799.reg.conf.buf_en      = val ?  1 : 0;
        ad7799_reg_conf_write( p->ad7799.reg.conf );
        nvm_write16( NVM_ADDR_AFE_ADC_REG_CONF, &p->ad7799.reg.conf.u16, 1 );
}


uint16_t
dev_afe_adc_chnl_get(                   const   dev_t *         p )
{
        return( p->ad7799.reg.conf.chnl );
}


void
dev_afe_adc_chnl_set(                           dev_t *         p,
                                        const   uint16_t        channel )
{
        p->ad7799.reg.conf.chnl        = (ad7799_chnl_t) channel;
        ad7799_reg_conf_write( p->ad7799.reg.conf );
        nvm_write16( NVM_ADDR_AFE_ADC_REG_CONF, &p->ad7799.reg.conf.u16, 1 );
}


uint16_t
dev_afe_adc_gain_get(                   const   dev_t *         p )
{
        return( p->ad7799.reg.conf.gain );
       
}


void
dev_afe_adc_gain_set(                           dev_t *         p,
                                        const   uint16_t        gain )
{
        p->ad7799.reg.conf.gain        = (ad7799_gain_t) gain;
        ad7799_reg_conf_write( p->ad7799.reg.conf );
        nvm_write16( NVM_ADDR_AFE_ADC_REG_CONF, &p->ad7799.reg.conf.u16, 1 );
}
