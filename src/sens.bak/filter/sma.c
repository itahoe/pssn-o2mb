/**
  * @file    sma.c
  * @brief   Simple Moving Average Filter
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdint.h>
#include "sma.h"



void
sma_init(                           sma_t *             p,
                                    sma_data_t *        buf,
                                    size_t              buf_sizeof )
{
        //memset ( p, 0, sizeof(sma_t) );
        memset ( buf, 0, sizeof(sma_data_t) * buf_sizeof );

        //p->buf          = buf;
        p->buf_sizeof   = buf_sizeof;
        p->idx          = 0;
        p->sum          = 0;
}


sma_data_t
sma_filter(                         sma_t *             p,
                                    sma_data_t          x )
{
        p->sum                  += x;
        p->sum                  -= p->buf[ p->idx ];
        p->buf[ p->idx ]        =  x;

        p->idx++;

        if( p->idx >= p->buf_sizeof )
        {
                p->idx                  = 0;
        }

        return( p->sum / p->buf_sizeof );
}


sma_data_t
sma_filter_multi(                   sma_t *             p,
                                    sma_data_t          x,
                                    size_t              order )
{
/*
    for( size_t i = 0; i < order; i++ )
    {
        x   = sma_filter( &p->meas.sma[ i], x );
    }
*/
    return( x );
}
