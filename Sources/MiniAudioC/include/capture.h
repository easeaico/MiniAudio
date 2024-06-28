#ifndef CAPTURE_H
#define CAPTURE_H

#include "audiodata.h"

extern int initAudioCaptureDevice(ma_encoding_format encodingFormat,
                                  ma_format format, ma_uint32 channels,
                                  ma_uint32 sampleRate, AudioData *audioData);

extern int startAudioCapturing();

extern int stopAudioCapturing();

extern void closeAudioCaptureDevice();

#endif
