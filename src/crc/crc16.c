/**
  * @file    crc16.c
  * @brief   CRC Routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "crc.h"


uint16_t
crc16_be_msbf(                  const   uint16_t        init,
                                const   uint16_t        poly,
                                const   uint8_t *       data,
                                        size_t          cnt )
{
        uint16_t        carry;
        uint16_t        crc     = init;


        while( cnt-- )
        {
                unsigned j = 8;

                printf( "%02X", *data );

                crc     ^= (uint16_t) *data++;

                while(j--)
                {
                        carry   =   crc & 0x0001;
                        crc     >>= 1;

                        if( carry )
                        {
                                crc     ^= poly;
                        }
                }
        }

        return( (crc >> 8) | (crc << 8) );
}
