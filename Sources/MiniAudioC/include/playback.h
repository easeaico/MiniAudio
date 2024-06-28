#ifndef PLAYBACK_H
#define PLAYBACK_H

#include "audiodata.h"

extern int initAudioPlackbackDevice(AudioData *audioData, ma_encoding_format encodingFormat, ma_format format, ma_uint32 channels, ma_uint32 sampleRate);

extern int startAudioPlaying();

extern int stopAudioPlaying();

extern void closeAudioPlaybackDevice();
#endif
