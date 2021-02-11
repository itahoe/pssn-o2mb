/**
  * @file    lm75.c
  * @brief   Sensor Temperature
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "config.h"
#include "lm75.h"
#include "bsp.h"


/**
  * @brief  
  * @param  None
  * @retval None
  */
void
lm75_init( void )
{
        lm75_x_init();
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int16_t
lm75_read(              const   lm75_reg_t      reg )
{
        uint16_t        data    = 0;


        data    =   lm75_x_read( (uint8_t) CFG_LM75_I2C_ADDR, reg );

        return( data );
}
