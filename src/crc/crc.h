/**
  * @file    crc.h
  * @brief   CRC
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef CRC_H
#define CRC_H


#include <stdint.h>
#include <stdio.h>


uint16_t
crc16_be_msbf(                  const   uint16_t        init,
                                const   uint16_t        poly,
                                const   uint8_t *       data,
                                        size_t          cnt );
#endif  //CRC_H
