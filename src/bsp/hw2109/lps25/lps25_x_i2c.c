/**
  * @file    bsp_lps25.c
  * @brief   Temperature Sensor Hardware Routins
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "config.h"
#include "stm32.h"


/*******************************************************************************
* READ
*******************************************************************************/
/**
  * @brief  
  * @param  None
  * @retval None
  */
int
lps25_x_read(                           const   uint16_t        reg_addr,
                                                uint8_t *       data,
                                                size_t          size )
{
        int             sts;


        sts     = stm32_i2c2_read( CONFIG_I2C_ADDR_LPS25, reg_addr, data, size );

        return( sts );   
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int
lps25_x_write(                          const   uint16_t        reg_addr,
                                                uint8_t *       data,
                                                uint8_t         cnt )
{
        int             sts;


        sts     = stm32_i2c2_write( CONFIG_I2C_ADDR_LPS25, reg_addr, data, 1 );

        return( sts );   
}
