#ifndef CAPTURE_H
#define CAPTURE_H

#include "miniaudio.h"

typedef struct {
  ma_uint8 *buffer;
  size_t size;
  size_t offset;
} AudioData;

extern int initAudioCaptureDevice(ma_encoding_format encodingFormat,
                                  ma_format format, ma_uint32 channels,
                                  ma_uint32 sampleRate, AudioData *audioData);

extern int startAudioCapturing();

extern int stopAudioCapturing();

extern void closeAudioCaptureDevice();

#endif
