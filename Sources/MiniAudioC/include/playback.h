#ifndef PLAYBACK_H
#define PLAYBACK_H

#include <stdio.h>

extern int initAudioPlackbackDevice(const void *pData, size_t dataSize);

extern int startAudioPlaying();

extern int stopAudioPlaying();

extern void closeAudioPlaybackDevice();
#endif
