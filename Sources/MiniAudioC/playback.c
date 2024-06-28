#include "include/playback.h"
#include "include/miniaudio.h"
#include <stdio.h>
#include <stdlib.h>

static ma_device playbackDevice;

static void data_callback(ma_device *pDevice, void *pOutput, const void *pInput,
                          ma_uint32 frameCount) {
   ma_audio_buffer* pAudioBuffer = (ma_audio_buffer*)pDevice->pUserData;
   ma_audio_buffer_read_pcm_frames(pAudioBuffer, pOutput, frameCount, MA_FALSE);
   (void)pInput;   /* Unused. */
}

int initAudioPlackbackDevice(AudioData *audioData) {
  ma_decoder_config decoderConfig = ma_decoder_config_init_default();
  ma_uint64 pFrameCount;
  void* ppPCMFrames;
  if (ma_decode_memory(audioData->buffer, audioData->size, &decoderConfig, &pFrameCount, &ppPCMFrames) != MA_SUCCESS) {
      printf("Failed to decode wave memory.\n");
      return -1;
  }

  ma_audio_buffer buffer;
  ma_audio_buffer_config bufferConfig;

  bufferConfig = ma_audio_buffer_config_init(decoderConfig.format, decoderConfig.channels, pFrameCount, ppPCMFrames, NULL);
  if (ma_audio_buffer_init(&bufferConfig, &buffer) != MA_SUCCESS) {
    printf("Failed to init audio buffer.\n");
    return -2;
  }


  ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = decoderConfig.format;
  deviceConfig.playback.channels = decoderConfig.channels;
  deviceConfig.sampleRate = decoderConfig.sampleRate;
  deviceConfig.dataCallback = data_callback;
  deviceConfig.pUserData = &buffer;

  if (ma_device_init(NULL, &deviceConfig, &playbackDevice) != MA_SUCCESS) {
    printf("Failed to open playback device.\n");
    return -3;
  }

  int duration = pFrameCount / decoderConfig.sampleRate + 1;
  return duration;
}

int startAudioPlaying() {
  if (ma_device_start(&playbackDevice) != MA_SUCCESS) {
    printf("Failed to start playback 1 device.\n");
    ma_device_uninit(&playbackDevice);
    return -4;
  }

  return 0;
}

void closeAudioPlaybackDevice() {
  ma_device_uninit(&playbackDevice);
}
