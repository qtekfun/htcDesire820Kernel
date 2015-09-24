/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PP_CONTENT_DECRYPTOR_H_
#define PPAPI_C_PRIVATE_PP_CONTENT_DECRYPTOR_H_

#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"



struct PP_DecryptTrackingInfo {
  uint32_t request_id;
  uint32_t buffer_id;
  int64_t timestamp;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_DecryptTrackingInfo, 16);

struct PP_DecryptSubsampleDescription {
  uint32_t clear_bytes;
  uint32_t cipher_bytes;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_DecryptSubsampleDescription, 8);

struct PP_EncryptedBlockInfo {
  struct PP_DecryptTrackingInfo tracking_info;
  uint32_t data_size;
  uint32_t data_offset;
  uint8_t key_id[64];
  uint32_t key_id_size;
  uint8_t iv[16];
  uint32_t iv_size;
  struct PP_DecryptSubsampleDescription subsamples[16];
  uint32_t num_subsamples;
  uint32_t padding;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_EncryptedBlockInfo, 248);

typedef enum {
  PP_DECRYPTEDFRAMEFORMAT_UNKNOWN = 0,
  PP_DECRYPTEDFRAMEFORMAT_YV12 = 1,
  PP_DECRYPTEDFRAMEFORMAT_I420 = 2
} PP_DecryptedFrameFormat;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_DecryptedFrameFormat, 4);

typedef enum {
  PP_DECRYPTEDSAMPLEFORMAT_UNKNOWN = 0,
  PP_DECRYPTEDSAMPLEFORMAT_U8 = 1,
  PP_DECRYPTEDSAMPLEFORMAT_S16 = 2,
  PP_DECRYPTEDSAMPLEFORMAT_S32 = 3,
  PP_DECRYPTEDSAMPLEFORMAT_F32 = 4,
  PP_DECRYPTEDSAMPLEFORMAT_PLANAR_S16 = 5,
  PP_DECRYPTEDSAMPLEFORMAT_PLANAR_F32 = 6
} PP_DecryptedSampleFormat;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_DecryptedSampleFormat, 4);

typedef enum {
  
  PP_DECRYPTRESULT_SUCCESS = 0,
  
  PP_DECRYPTRESULT_DECRYPT_NOKEY = 1,
  
  PP_DECRYPTRESULT_NEEDMOREDATA = 2,
  
  PP_DECRYPTRESULT_DECRYPT_ERROR = 3,
  
  PP_DECRYPTRESULT_DECODE_ERROR = 4
} PP_DecryptResult;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_DecryptResult, 4);

struct PP_DecryptedBlockInfo {
  PP_DecryptResult result;
  uint32_t data_size;
  struct PP_DecryptTrackingInfo tracking_info;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_DecryptedBlockInfo, 24);

typedef enum {
  PP_DECRYPTEDFRAMEPLANES_Y = 0,
  PP_DECRYPTEDFRAMEPLANES_U = 1,
  PP_DECRYPTEDFRAMEPLANES_V = 2
} PP_DecryptedFramePlanes;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_DecryptedFramePlanes, 4);

struct PP_DecryptedFrameInfo {
  PP_DecryptResult result;
  PP_DecryptedFrameFormat format;
  int32_t plane_offsets[3];
  int32_t strides[3];
  int32_t width;
  int32_t height;
  struct PP_DecryptTrackingInfo tracking_info;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_DecryptedFrameInfo, 56);

struct PP_DecryptedSampleInfo {
  PP_DecryptResult result;
  PP_DecryptedSampleFormat format;
  uint32_t data_size;
  uint32_t padding;
  struct PP_DecryptTrackingInfo tracking_info;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_DecryptedSampleInfo, 32);

typedef enum {
  PP_AUDIOCODEC_UNKNOWN = 0,
  PP_AUDIOCODEC_VORBIS = 1,
  PP_AUDIOCODEC_AAC = 2
} PP_AudioCodec;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_AudioCodec, 4);

struct PP_AudioDecoderConfig {
  PP_AudioCodec codec;
  int32_t channel_count;
  int32_t bits_per_channel;
  int32_t samples_per_second;
  uint32_t request_id;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_AudioDecoderConfig, 20);

typedef enum {
  PP_VIDEOCODEC_UNKNOWN = 0,
  PP_VIDEOCODEC_VP8 = 1,
  PP_VIDEOCODEC_H264 = 2
} PP_VideoCodec;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_VideoCodec, 4);

typedef enum {
  PP_VIDEOCODECPROFILE_UNKNOWN = 0,
  PP_VIDEOCODECPROFILE_VP8_MAIN = 1,
  PP_VIDEOCODECPROFILE_H264_BASELINE = 2,
  PP_VIDEOCODECPROFILE_H264_MAIN = 3,
  PP_VIDEOCODECPROFILE_H264_EXTENDED = 4,
  PP_VIDEOCODECPROFILE_H264_HIGH = 5,
  PP_VIDEOCODECPROFILE_H264_HIGH_10 = 6,
  PP_VIDEOCODECPROFILE_H264_HIGH_422 = 7,
  PP_VIDEOCODECPROFILE_H264_HIGH_444_PREDICTIVE = 8
} PP_VideoCodecProfile;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_VideoCodecProfile, 4);

struct PP_VideoDecoderConfig {
  PP_VideoCodec codec;
  PP_VideoCodecProfile profile;
  PP_DecryptedFrameFormat format;
  int32_t width;
  int32_t height;
  uint32_t request_id;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_VideoDecoderConfig, 24);

typedef enum {
  PP_DECRYPTORSTREAMTYPE_AUDIO = 0,
  PP_DECRYPTORSTREAMTYPE_VIDEO = 1
} PP_DecryptorStreamType;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_DecryptorStreamType, 4);

#endif  

