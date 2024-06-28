#ifndef AUDIO_DATA_H
#define AUDIO_DATA_H

#include <stdio.h>
#include "miniaudio.h"

typedef struct {
  ma_uint8 *buffer;
  size_t size;
  size_t offset;
} AudioData;

#endif
