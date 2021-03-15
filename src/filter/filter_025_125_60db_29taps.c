/**
  * @file    filter.c
  * @brief   Filter
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdint.h>
#include "filter_025_125_60db_29taps.h"


static int32_t filter_taps[FILTER_TAP_NUM] = {
  -2221204,
  -5241173,
  -9709873,
  -14364886,
  -17040451,
  -14696765,
  -4014235,
  17693056,
  51469810,
  95932470,
  147000146,
  198332663,
  242454975,
  272312462,
  282873775,
  272312462,
  242454975,
  198332663,
  147000146,
  95932470,
  51469810,
  17693056,
  -4014235,
  -14696765,
  -17040451,
  -14364886,
  -9709873,
  -5241173,
  -2221204
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
    acc += (uint64_t)f->history[index] * filter_taps[i];
  };
  return acc >> 32;
}
