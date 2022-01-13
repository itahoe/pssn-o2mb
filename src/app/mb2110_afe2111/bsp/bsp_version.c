/**
  * @file    bsp_version.c
  * @brief   Board Support Package - Version
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include <stdbool.h>
//#include "bsp.h"
#include "stm32.h"
#include "app_error.h"


#define YEAR_H          (__DATE__[10])
#define YEAR_L          (__DATE__[ 9])

#define MONTH_H         ((__DATE__[2] == 'n' ? (__DATE__ [1] == 'a'? '1' : '6') \
                        : __DATE__[2] == 'b' ? '2' \
                        : __DATE__[2] == 'r' ? (__DATE__ [0] == 'M'? '3' : '4') \
                        : __DATE__[2] == 'y' ? '5' \
                        : __DATE__[2] == 'l' ? '7' \
                        : __DATE__[2] == 'g' ? '8' \
                        : __DATE__[2] == 'p' ? '9' \
                        : __DATE__[2] == 't' ? '0' \
                        : __DATE__[2] == 'v' ? '1' : '2'))

#define MONTH_L         ((__DATE__[2] == 'n' ? (__DATE__ [1] == 'a'? '0' : '0') \
                        : __DATE__[2] == 'b' ? '0' \
                        : __DATE__[2] == 'r' ? (__DATE__ [0] == 'M'? '0' : '0') \
                        : __DATE__[2] == 'y' ? '0' \
                        : __DATE__[2] == 'l' ? '0' \
                        : __DATE__[2] == 'g' ? '0' \
                        : __DATE__[2] == 'p' ? '0' \
                        : __DATE__[2] == 't' ? '1' \
                        : __DATE__[2] == 'v' ? '1' : '1'))

#define DAY_H           (__DATE__[5])

#define DAY_L           (__DATE__[4]==' ' ? '0' : __DATE__[4])


/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
bsp_version_hw_get(                             const   size_t          idx  )
{
        union
        {
                uint16_t        u16[ 2];
                uint8_t         u08[ 4];
        } ver;

        const   char    hardware_ver[]  = { '2', '0', '2', '6', };


        ver.u08[ 3]     = hardware_ver[ 0];
        ver.u08[ 2]     = hardware_ver[ 1];
        ver.u08[ 1]     = hardware_ver[ 2];
        ver.u08[ 0]     = hardware_ver[ 3];

        if( idx < 2 )
        {
                return( ver.u16[ idx ] );
        }

        return( 0 );
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
uint16_t
bsp_version_fw_get(                             const   size_t          idx  )
{
        union
        {
                uint16_t        u16[ 3];
                uint8_t         u08[ 6];
        } ver;


        ver.u08[ 0]     = YEAR_H;
        ver.u08[ 1]     = YEAR_L;
        ver.u08[ 2]     = MONTH_H;
        ver.u08[ 3]     = MONTH_L;
        ver.u08[ 4]     = DAY_H;
        ver.u08[ 5]     = DAY_L;

        if( idx < 3 )
        {
                return( ver.u16[ idx ] );
        }

        return( 0 );
}
