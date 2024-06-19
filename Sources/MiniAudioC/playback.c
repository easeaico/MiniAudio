#include "include/playback.h"
#include "include/miniaudio.h"
#include <stdio.h>
#include <stdlib.h>

ma_device playbackDevice;
ma_decoder playbackDecoder;

void decode_data_callback(ma_device *pDevice, void *pOutput, const void *pInput,
                          ma_uint32 frameCount) {
  ma_decoder *pDecoder = (ma_decoder *)pDevice->pUserData;
  if (pDecoder == NULL) {
    return;
  }

  ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

  (void)pInput;
}

int initAudioPlackbackDevice(const void *pData, size_t dataSize) {
  ma_result result;
  ma_device_config deviceConfig;

  result = ma_decoder_init_memory(pData, dataSize, NULL, &playbackDecoder);
  if (result != MA_SUCCESS) {
    return -2;
  }

  deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = playbackDecoder.outputFormat;
  deviceConfig.playback.channels = playbackDecoder.outputChannels;
  deviceConfig.sampleRate = playbackDecoder.outputSampleRate;
  deviceConfig.dataCallback = decode_data_callback;
  deviceConfig.pUserData = &playbackDecoder;

  if (ma_device_init(NULL, &deviceConfig, &playbackDevice) != MA_SUCCESS) {
    printf("Failed to open playback device.\n");
    ma_decoder_uninit(&playbackDecoder);
    return -3;
  }

  return 0;
}

int startAudioPlaying() {
  if (ma_device_start(&playbackDevice) != MA_SUCCESS) {
    printf("Failed to start playback 1 device.\n");
    ma_device_uninit(&playbackDevice);
    ma_decoder_uninit(&playbackDecoder);
    return -4;
  }

  return 0;
}

int stopAudioPlaying() {
  ma_result result;

  result = ma_device_stop(&playbackDevice);
  if (result != MA_SUCCESS) {
    ma_device_uninit(&playbackDevice);
    printf("Failed to start device.\n");
    return -3;
  }

  return 0;
}

void closeAudioPlaybackDevice() {
  ma_device_uninit(&playbackDevice);
  ma_decoder_uninit(&playbackDecoder);
}
