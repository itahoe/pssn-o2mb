/**
  * @file    ema.c
  * @brief   Exponential Moving Average Filter
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdint.h>
#include "ema.h"



void
ema_init(                           ema_t *             p,
                                    float               decay   )
{
    p->b    = 1 - decay;
    p->y    = 0;
}


/*
    Apply the filter function for each new input sample x,
    resulting in a new output sample y of the filtered signal:
    y = ema_fulter(x)
*/
float
ema_filter(                         ema_t *             p,
                                    float               x )
{
    p->y    += p->b * (x - p->y);
    return( p->y );
}
