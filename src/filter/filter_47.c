/**
  * @file    filter.c
  * @brief   Filter
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdint.h>
#include "filter_47.h"


static int32_t filter_taps[FILTER_TAP_NUM] = {
  2320239,
  4314486,
  7388747,
  10794063,
  13758944,
  15250248,
  14162616,
  9600485,
  1190771,
  -10620517,
  -24401873,
  -37738078,
  -47472779,
  -50161069,
  -42685620,
  -22923926,
  9668035,
  53687119,
  105740430,
  160764299,
  212683814,
  255306560,
  283297611,
  293052422,
  283297611,
  255306560,
  212683814,
  160764299,
  105740430,
  53687119,
  9668035,
  -22923926,
  -42685620,
  -50161069,
  -47472779,
  -37738078,
  -24401873,
  -10620517,
  1190771,
  9600485,
  14162616,
  15250248,
  13758944,
  10794063,
  7388747,
  4314486,
  2320239
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
