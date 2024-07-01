#ifndef AUDIO_DATA_H
#define AUDIO_DATA_H

#include "miniaudio.h"
#include <stdio.h>

typedef struct {
  ma_uint8 *buffer;
  size_t size;
  size_t offset;
} AudioData;

#endif
