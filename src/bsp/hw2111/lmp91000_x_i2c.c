/**
  * @file    lmp91000_x_i2c.c
  * @brief   LMP91000 Hardware Routins
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "config.h"
#include "stm32.h"


/**
  * @brief  
  * @param  None
  * @retval None
  */
uint8_t
lmp91000_x_read(                           const   uint8_t         reg_addr )
{
        int             sts;
        uint8_t         data;


        //sts     = stm32_i2c1_read( CONFIG_I2C_ADDR_LMP91000, reg_addr, &data, 1 );
        sts     = stm32_i2c2_read( CONFIG_I2C_ADDR_LMP91000, reg_addr, &data, 1 );

 
        if( sts != 0 )
        {
                //app_error_handler();
        }

        return( data );   
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
lmp91000_x_write(                       const   uint8_t         reg_addr,
                                                uint8_t         data )
{
        int             sts;


        //sts     = stm32_i2c1_write( CONFIG_I2C_ADDR_LMP91000, reg_addr, &data, 1 );
        sts     = stm32_i2c2_write( CONFIG_I2C_ADDR_LMP91000, reg_addr, &data, 1 );


        if( sts != 0 )
        {
                //app_error_handler();
        }
}
