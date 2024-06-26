#ifndef PLAYBACK_H
#define PLAYBACK_H

#include "audiodata.h"

typedef struct PlaybackContext {
  ma_device device;
  ma_audio_buffer audioBuffer;
  ma_uint32 duration;
} PlaybackContext;

extern int initDeviceOrUpdateData(PlaybackContext *ctx, AudioData *audioData);

extern int startAudioPlaying(PlaybackContext *ctx);

extern int stopAudioPlaying(PlaybackContext *ctx);

extern void closePlaybackDevice(PlaybackContext *ctx);
#endif
