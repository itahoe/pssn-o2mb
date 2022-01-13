/**
  * @file    bsp_sys.c
  * @brief   System Routins
  * @author  Igor T. <research.tahoe@gmail.com>
  */


//#include "bsp.h"
#include "stm32.h"


/*******************************************************************************
* SYS RESET
*******************************************************************************/
/**
 * @brief       Reset System
 */
void
bsp_sys_reset( void )
{
        NVIC_SystemReset();
}


/*******************************************************************************
* SYS ID
*******************************************************************************/
/**
 * @brief       Get Device ID
 */
uint16_t
bsp_sys_device_id(                              const   size_t          idx  )
{
        union
        {
                uint16_t        u16[ 2];
                uint8_t         u08[ 4];
        } u;

        const   uint8_t id[]  = { 0x0A, 0x0B, 0x0C, 0x0D, };


        u.u08[ 0]     = id[ 0];
        u.u08[ 1]     = id[ 1];
        u.u08[ 2]     = id[ 2];
        u.u08[ 3]     = id[ 3];

        switch( idx )
        {
                case 0:         return( u.u16[ 0 ] );
                case 1:         return( u.u16[ 1 ] );
                default:
                        return( 0 );
        }
}


/**
 * @brief       Get Hardware ID
 */
uint16_t
bsp_sys_hardware_id(                            const   size_t          idx  )
{
        const   uint8_t         id[]  = { '2', '0', '0', '3', };

        switch( idx )
        {
                //case 0:         return( (id[ 0] <<  8) | (id[ 1] & 0xFF) );
                //case 1:         return( (id[ 2] <<  8) | (id[ 3] & 0xFF) );
                case 0:         return( (id[ 1] <<  8) | (id[ 0] & 0xFF) );
                case 1:         return( (id[ 3] <<  8) | (id[ 2] & 0xFF) );
                default:
                        return( 0x0000 );
        }
}


/*******************************************************************************
* SYS FIRMWARE
*******************************************************************************/
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
 * @brief       Get Firmware Version
 */
uint16_t
bsp_sys_firmware_id(                            const   size_t          idx  )
{
        union
        {
                uint16_t        u16[ 3];
                uint8_t         u08[ 6];
        } ver;


        //ver.u08[ 0]     = YEAR_H;
        //ver.u08[ 1]     = YEAR_L;
        //ver.u08[ 2]     = MONTH_H;
        //ver.u08[ 3]     = MONTH_L;
        //ver.u08[ 4]     = DAY_H;
        //ver.u08[ 5]     = DAY_L;

        ver.u08[ 0]     = YEAR_L;
        ver.u08[ 1]     = YEAR_H;
        ver.u08[ 2]     = MONTH_L;
        ver.u08[ 3]     = MONTH_H;
        ver.u08[ 4]     = DAY_L;
        ver.u08[ 5]     = DAY_H;

        if( idx < 3 )
        {
                return( ver.u16[ idx ] );
        }

        return( 0 );
}


/**
 * @brief       Get Serial Number
 */
uint16_t
bsp_sys_serial_num(                             const   size_t          idx )
{
        uint16_t        u16;


        switch( idx )
        {
                case 0:         u16     = 0x0011;       break;
                case 1:         u16     = 0x2233;       break;
                case 2:         u16     = 0x4455;       break;
                case 3:         u16     = 0x6677;       break;
                case 4:         u16     = 0x8899;       break;
                case 5:         u16     = 0xAABB;       break;
                case 6:         u16     = 0xCCDD;       break;
                case 7:         u16     = 0xEEFF;       break;
                default:        u16     = 0xFFFF;       break;
        }

        return( u16 );
}
