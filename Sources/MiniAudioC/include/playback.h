#ifndef PLAYBACK_H
#define PLAYBACK_H

#include "audiodata.h"

extern int initAudioPlackbackDevice(AudioData *audioData);

extern int startAudioPlaying();

extern int stopAudioPlaying();

extern void closeAudioPlaybackDevice();
#endif
