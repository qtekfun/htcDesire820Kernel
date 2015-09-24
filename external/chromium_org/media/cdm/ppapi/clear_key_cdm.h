// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CDM_PPAPI_CLEAR_KEY_CDM_H_
#define MEDIA_CDM_PPAPI_CLEAR_KEY_CDM_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "media/cdm/aes_decryptor.h"
#include "media/cdm/ppapi/clear_key_cdm_common.h"

#if 0
#define CLEAR_KEY_CDM_USE_FAKE_AUDIO_DECODER
#endif

namespace media {
class CdmVideoDecoder;
class DecoderBuffer;
class FFmpegCdmAudioDecoder;

class ClearKeyCdm : public ClearKeyCdmInterface {
 public:
  explicit ClearKeyCdm(Host* host, bool is_decrypt_only);
  virtual ~ClearKeyCdm();

  
  virtual cdm::Status GenerateKeyRequest(
      const char* type, uint32_t type_size,
      const uint8_t* init_data, uint32_t init_data_size) OVERRIDE;
  virtual cdm::Status AddKey(const char* session_id,
                             uint32_t session_id_size,
                             const uint8_t* key,
                             uint32_t key_size,
                             const uint8_t* key_id,
                             uint32_t key_id_size) OVERRIDE;
  virtual cdm::Status CancelKeyRequest(const char* session_id,
                                       uint32_t session_id_size) OVERRIDE;
  virtual void TimerExpired(void* context) OVERRIDE;
  virtual cdm::Status Decrypt(const cdm::InputBuffer& encrypted_buffer,
                              cdm::DecryptedBlock* decrypted_block) OVERRIDE;
  virtual cdm::Status InitializeAudioDecoder(
      const cdm::AudioDecoderConfig& audio_decoder_config) OVERRIDE;
  virtual cdm::Status InitializeVideoDecoder(
      const cdm::VideoDecoderConfig& video_decoder_config) OVERRIDE;
  virtual void DeinitializeDecoder(cdm::StreamType decoder_type) OVERRIDE;
  virtual void ResetDecoder(cdm::StreamType decoder_type) OVERRIDE;
  virtual cdm::Status DecryptAndDecodeFrame(
      const cdm::InputBuffer& encrypted_buffer,
      cdm::VideoFrame* video_frame) OVERRIDE;
  virtual cdm::Status DecryptAndDecodeSamples(
      const cdm::InputBuffer& encrypted_buffer,
      cdm::AudioFrames* audio_frames) OVERRIDE;
  virtual void Destroy() OVERRIDE;
  virtual void OnPlatformChallengeResponse(
      const cdm::PlatformChallengeResponse& response) OVERRIDE;
  virtual void OnQueryOutputProtectionStatus(
      uint32_t link_mask, uint32_t output_protection_mask) OVERRIDE;

 private:
  
  
  class Client {
   public:
    
    
    enum Status {
      kNone = 0,
      kCreated = 1 << 0,
      kMessage = 1 << 1,
      kReady = 1 << 2,
      kClosed = 1 << 3,
      kError = 1 << 4
    };

    Client();
    virtual ~Client();

    Status status() { return status_; }
    const std::string& web_session_id() { return web_session_id_; }
    const std::vector<uint8>& message() { return message_; }
    const std::string& destination_url() { return destination_url_; }
    MediaKeys::KeyError error_code() { return error_code_; }
    int system_code() { return system_code_; }

    
    void Reset();

    void OnSessionCreated(uint32 session_id, const std::string& web_session_id);
    void OnSessionMessage(uint32 session_id,
                          const std::vector<uint8>& message,
                          const std::string& destination_url);
    void OnSessionReady(uint32 session_id);
    void OnSessionClosed(uint32 session_id);
    void OnSessionError(uint32 session_id,
                        MediaKeys::KeyError error_code,
                        int system_code);

   private:
    Status status_;
    std::string web_session_id_;
    std::vector<uint8> message_;
    std::string destination_url_;
    MediaKeys::KeyError error_code_;
    int system_code_;
  };

  
  void ScheduleNextHeartBeat();

  
  
  
  
  
  
  
  
  cdm::Status DecryptToMediaDecoderBuffer(
      const cdm::InputBuffer& encrypted_buffer,
      scoped_refptr<DecoderBuffer>* decrypted_buffer);

#if defined(CLEAR_KEY_CDM_USE_FAKE_AUDIO_DECODER)
  int64 CurrentTimeStampInMicroseconds() const;

  
  
  int GenerateFakeAudioFramesFromDuration(int64 duration_in_microseconds,
                                          cdm::AudioFrames* audio_frames) const;

  
  
  cdm::Status GenerateFakeAudioFrames(int64 timestamp_in_microseconds,
                                      cdm::AudioFrames* audio_frames);
#endif  

  Client client_;
  AesDecryptor decryptor_;

  
  
  base::Lock client_lock_;

  ClearKeyCdmHost* host_;

  const bool is_decrypt_only_;

  std::string heartbeat_session_id_;
  std::string next_heartbeat_message_;

  
  int64 timer_delay_ms_;

  
  
  bool timer_set_;

#if defined(CLEAR_KEY_CDM_USE_FAKE_AUDIO_DECODER)
  int channel_count_;
  int bits_per_channel_;
  int samples_per_second_;
  int64 output_timestamp_base_in_microseconds_;
  int total_samples_generated_;
#endif  

#if defined(CLEAR_KEY_CDM_USE_FFMPEG_DECODER)
  scoped_ptr<FFmpegCdmAudioDecoder> audio_decoder_;
#endif  

  scoped_ptr<CdmVideoDecoder> video_decoder_;

  DISALLOW_COPY_AND_ASSIGN(ClearKeyCdm);
};

}  

#endif  
