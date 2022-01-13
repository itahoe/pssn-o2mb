/**
  * @file    filter.c
  * @brief   Filter
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef FILTER_H_
#define FILTER_H_

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 1 Hz

fixed point precision: 32 bits

* 0 Hz - 0.05 Hz
  gain = 1
  desired ripple = 1 dB
  actual ripple = n/a

* 0.1 Hz - 0.5 Hz
  gain = 0
  desired attenuation = -60 dB
  actual attenuation = n/a

*/

#define FILTER_TAP_NUM 47

typedef struct {
  int32_t history[FILTER_TAP_NUM];
  unsigned int last_index;
} Filter;

void Filter_init(Filter* f);
void Filter_put(Filter* f, int32_t input);
int32_t Filter_get(Filter* f);

#endif