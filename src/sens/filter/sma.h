/**
  * @file    sma.h
  * @brief   Simple Moving Average Filter
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef SMA_H
#define SMA_H


#include <string.h>
#include "config.h"


typedef int32_t         sma_data_t;

typedef struct  sma_s
{
    sma_data_t          sum;
    //sma_data_t *        buf;
    sma_data_t          buf[CONF_SENS_LPF_FCUT_MAX];
    size_t              buf_sizeof;
    size_t              idx;
    size_t              order;
} sma_t;


void
sma_init(                           sma_t *             p,
                                    sma_data_t *        buf,
                                    size_t              buf_len );


sma_data_t
sma_filter(                         sma_t *             p,
                                    sma_data_t          x );


sma_data_t
sma_filter_multi(                   sma_t *             p,
                                    sma_data_t          x,
                                    size_t              order );


#endif  //SMA_H
