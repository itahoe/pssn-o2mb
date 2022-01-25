/**
  * @file    ema.h
  * @brief   Exponential Moving Average Filter
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef EMA_H
#define EMA_H


typedef struct  ema_s
{
    float   b;  //b coeffecient
    float   y;  //filter output data
} ema_t;


void
ema_init(                           ema_t *             p,
                                    float               decay   );


float
ema_filter(                         ema_t *             p,
                                    float               x );


#endif  //EMA_H
