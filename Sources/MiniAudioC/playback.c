#include "include/playback.h"
#include "include/miniaudio.h"
#include <stdio.h>
#include <stdlib.h>

static void data_callback(ma_device *pDevice, void *pOutput, const void *pInput,
                          ma_uint32 frameCount) {
  ma_audio_buffer *pAudioBuffer = (ma_audio_buffer *)pDevice->pUserData;
  ma_audio_buffer_read_pcm_frames(pAudioBuffer, pOutput, frameCount, MA_FALSE);
  (void)pInput; /* Unused. */
}

int initPlackbackDevice(PlaybackContext *ctx, AudioData *audioData) {
  ma_decoder_config decoderConfig = ma_decoder_config_init_default();
  ma_uint64 pFrameCount;
  void *ppPCMFrames;
  if (ma_decode_memory(audioData->buffer, audioData->size, &decoderConfig,
                       &pFrameCount, &ppPCMFrames) != MA_SUCCESS) {
    printf("Failed to decode memory.\n");
    return -1;
  }

  ctx->duration = (ma_uint32)(pFrameCount / decoderConfig.sampleRate + 1);
    
  ma_allocation_callbacks allocationCallbacks;
  ma_audio_buffer_config bufferConfig = ma_audio_buffer_config_init(
      decoderConfig.format, decoderConfig.channels, pFrameCount, ppPCMFrames,
      &allocationCallbacks);
  if (ma_audio_buffer_init(&bufferConfig, &ctx->audioBuffer) != MA_SUCCESS) {
    printf("Failed to init audio buffer.\n");
    return -2;
  }

  ma_device_config deviceConfig =
      ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = decoderConfig.format;
  deviceConfig.playback.channels = decoderConfig.channels;
  deviceConfig.sampleRate = decoderConfig.sampleRate;
  deviceConfig.dataCallback = data_callback;
  deviceConfig.pUserData = &ctx->audioBuffer;

  if (ma_device_init(NULL, &deviceConfig, &ctx->device) != MA_SUCCESS) {
    printf("Failed to open playback device.\n");
    return -3;
  }

  return 0;
}

int startAudioPlaying(PlaybackContext *ctx) {
  if (ma_device_start(&ctx->device) != MA_SUCCESS) {
    printf("Failed to start playback 1 device.\n");
    ma_device_uninit(&ctx->device);
    return -4;
  }

  return 0;
}

int updateAudioData(PlaybackContext *ctx, AudioData *data) {
    ma_decoder_config decoderConfig = ma_decoder_config_init_default();
    ma_uint64 pFrameCount;
    void *ppPCMFrames;
    if (ma_decode_memory(data->buffer, data->size, &decoderConfig,
                         &pFrameCount, &ppPCMFrames) != MA_SUCCESS) {
      printf("Failed to decode memory.\n");
      return -1;
    }

    ma_allocation_callbacks allocationCallbacks;
    ma_audio_buffer_config bufferConfig = ma_audio_buffer_config_init(
        decoderConfig.format, decoderConfig.channels, pFrameCount, ppPCMFrames,
        &allocationCallbacks);
    if (ma_audio_buffer_init(&bufferConfig, &ctx->audioBuffer) != MA_SUCCESS) {
      printf("Failed to init audio buffer.\n");
      return -2;
    }
    
    ctx->duration = (ma_uint32)(pFrameCount / decoderConfig.sampleRate + 1);
}

int stopAudioPlaying(PlaybackContext *ctx) {
    if (ma_device_stop(&ctx->device) != MA_SUCCESS) {
      printf("Failed to start playback 1 device.\n");
      ma_device_uninit(&ctx->device);
      return -4;
    }
    
    ma_audio_buffer_uninit(&ctx->audioBuffer);
    return 0;
}

void closePlaybackDevice(PlaybackContext *ctx) {
    ma_device_uninit(&ctx->device);
}
