/**
  * @file    filter.c
  * @brief   Filter
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdint.h>
#include "filter_13.h"


static int32_t filter_taps[FILTER_TAP_NUM] = {
  -8047091,
  -44683441,
  -83719935,
  -17417543,
  240970712,
  584702207,
  748084598,
  584702207,
  240970712,
  -17417543,
  -83719935,
  -44683441,
  -8047091
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
