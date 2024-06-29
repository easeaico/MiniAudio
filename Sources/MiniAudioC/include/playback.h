#ifndef PLAYBACK_H
#define PLAYBACK_H

#include "audiodata.h"

typedef struct PlaybackContext {
    ma_device device;
    ma_audio_buffer audioBuffer;
} PlaybackContext;

extern int initAudioPlackbackDevice(PlaybackContext *ctx, AudioData *audioData);

extern int startAudioPlaying(PlaybackContext *ctx);

extern void closeAudioPlaybackDevice(PlaybackContext *ctx);
#endif
