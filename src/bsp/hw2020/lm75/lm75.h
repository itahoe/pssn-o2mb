/**
  * @file    lm75.h
  * @brief   Sensor Temperature
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef LM75_H
#define LM75_H


#include <stdint.h>


typedef enum    lm75_reg_e
{
        LM75_REG_TEMP           = 0x00,
        LM75_REG_CONF           = 0x01,
        LM75_REG_THYS           = 0x02,
        LM75_REG_TOS            = 0x03,
} lm75_reg_t;


/*******************************************************************************
*
*******************************************************************************/
void    lm75_init( void );

int16_t lm75_read(              const   lm75_reg_t      reg );


/*******************************************************************************
*
*******************************************************************************/
void
lm75_x_init( void );


uint8_t
lm75_x_read(                            const   uint8_t         addr,
                                        const   uint8_t         reg );

void
lm75_x_write(                           const   uint8_t         addr,
                                        const   uint8_t         reg,
                                                uint8_t         value );


#endif  //LM75_H
