#include "include/capture.h"
#include "include/miniaudio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ma_result encoder_write_proc(ma_encoder *pEncoder, const void *pBufferIn,
                                    size_t bytesToWrite,
                                    size_t *pBytesWritten) {
  AudioData *data = (AudioData *)pEncoder->pUserData;

  if (data->offset < data->size) { // on seek
    memcpy(data->buffer + data->offset, pBufferIn,
           sizeof(ma_uint8) * bytesToWrite);
    data->offset = data->size;
  } else { // on write
    ma_uint8 *newBuffer = (ma_uint8 *)realloc(
        data->buffer, sizeof(ma_uint8) * (bytesToWrite + data->offset));
    memcpy(newBuffer + data->offset, pBufferIn,
           sizeof(ma_uint8) * bytesToWrite);

    data->buffer = newBuffer;
    *pBytesWritten += bytesToWrite;
    data->offset += bytesToWrite;
    data->size = data->offset;
  }

  return MA_SUCCESS;
}

ma_result my_encoder_seek_proc(ma_encoder *pEncoder, ma_int64 offset,
                               ma_seek_origin origin) {
  AudioData *data = (AudioData *)pEncoder->pUserData;
  data->offset = offset;

  return MA_SUCCESS;
}

void encode_data_callback(ma_device *pDevice, void *pOutput, const void *pInput,
                          ma_uint32 frameCount) {
  ma_encoder *pEncoder = (ma_encoder *)pDevice->pUserData;
  ma_result result =
      ma_encoder_write_pcm_frames(pEncoder, pInput, frameCount, NULL);
  if (result != MA_SUCCESS) {
    printf("Failed to write pcm frames for capturing.\n");
  }

  (void)pOutput;
}

int initCaptureDevice(CaptureContext *ctx, ma_encoding_format encodingFormat,
                      ma_format format, ma_uint32 channels,
                      ma_uint32 sampleRate, AudioData *audioData) {
  ma_result result;
  ma_encoder_config encoderConfig;
  ma_device_config deviceConfig;

  encoderConfig =
      ma_encoder_config_init(encodingFormat, format, channels, sampleRate);
  if (ma_encoder_init(encoder_write_proc, my_encoder_seek_proc,
                      (void *)audioData, &encoderConfig,
                      &ctx->encoder) != MA_SUCCESS) {
    return -1;
  }

  deviceConfig = ma_device_config_init(ma_device_type_capture);
  deviceConfig.capture.format = ctx->encoder.config.format;
  deviceConfig.capture.channels = ctx->encoder.config.channels;
  deviceConfig.sampleRate = ctx->encoder.config.sampleRate;
  deviceConfig.dataCallback = encode_data_callback;
  deviceConfig.pUserData = &ctx->encoder;

  result = ma_device_init(NULL, &deviceConfig, &ctx->device);
  if (result != MA_SUCCESS) {
    printf("Failed to initialize capture device.\n");
    return -2;
  }

  return 0;
}

int startAudioCapturing(CaptureContext *ctx) {
  ma_result result;

  result = ma_device_start(&ctx->device);
  if (result != MA_SUCCESS) {
    printf("Failed to start device for capturing.\n");
    ma_encoder_uninit(&ctx->encoder);
    ma_device_uninit(&ctx->device);
    return -3;
  }

  return 0;
}

void closeCaptureDevice(CaptureContext *ctx) {
  ma_device_uninit(&ctx->device);
  ma_encoder_uninit(&ctx->encoder);
}
