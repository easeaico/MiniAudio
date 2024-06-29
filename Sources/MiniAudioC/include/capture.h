#ifndef CAPTURE_H
#define CAPTURE_H

#include "audiodata.h"

typedef struct CaptureContext {
    ma_device device;
    ma_encoder encoder;
} CaptureContext;

extern int initAudioCaptureDevice(CaptureContext *ctx, ma_encoding_format encodingFormat,
                                  ma_format format, ma_uint32 channels,
                                  ma_uint32 sampleRate, AudioData *audioData);

extern int startAudioCapturing(CaptureContext *ctx);

extern void closeAudioCaptureDevice(CaptureContext *ctx);

#endif
