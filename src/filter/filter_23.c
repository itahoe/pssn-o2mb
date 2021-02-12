/**
  * @file    filter.c
  * @brief   Filter
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdint.h>
#include "filter_23.h"

#include "filter_23.h"

static int32_t filter_taps[FILTER_TAP_NUM] = {
  6620124,
  17726227,
  25423917,
  13990225,
  -25564068,
  -78544892,
  -102502671,
  -47777619,
  105312374,
  319345264,
  508451173,
  583983495,
  508451173,
  319345264,
  105312374,
  -47777619,
  -102502671,
  -78544892,
  -25564068,
  13990225,
  25423917,
  17726227,
  6620124
};

void Filter_init(Filter* f) {
  int i;
  for(i = 0; i < FILTER_TAP_NUM; ++i)
    f->history[i] = 0;
  f->last_index = 0;
}

void Filter_put(Filter* f, int32_t input) {
  f->history[f->last_index++] = input;
  if(f->last_index == FILTER_TAP_NUM)
    f->last_index = 0;
}

int32_t Filter_get(Filter* f) {
  int64_t acc = 0;
  int index = f->last_index, i;
  for(i = 0; i < FILTER_TAP_NUM; ++i) {
    index = index != 0 ? index-1 : FILTER_TAP_NUM-1;
    acc += (int64_t)f->history[index] * filter_taps[i];
  };
  return acc >> 32;
}
