/**
  * @file    lm75.c
  * @brief   Sensor Temperature
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include <stdbool.h>
#include "lm75.h"
#include "bsp.h"


#define LM75_ADDR               0x90
/*
#define LM75_REG_TEMP           0x00
#define LM75_REG_CONF           0x01
#define LM75_REG_THYS           0x02
#define LM75_REG_TOS            0x03
*/


void
lm75_init( void )
{
        bsp_lm75_init();
}


int16_t
lm75_read(              const   lm75_reg_t      reg )
{
        uint16_t        data    = 0;


        //data    =   bsp_lm75_read_u08( LM75_ADDR, LM75_REG_TEMP );
        //data    =   bsp_lm75_read_u16( addr, LM75_REG_TEMP );
        data    =   bsp_lm75_read( LM75_ADDR, reg );

        return( data );
}
