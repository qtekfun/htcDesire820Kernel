// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_PRIVATE_CONTENT_DECRYPTOR_PRIVATE_H_
#define PPAPI_CPP_PRIVATE_CONTENT_DECRYPTOR_PRIVATE_H_

#include "ppapi/c/private/pp_content_decryptor.h"
#include "ppapi/c/private/ppb_content_decryptor_private.h"
#include "ppapi/c/private/ppp_content_decryptor_private.h"

#include "ppapi/cpp/dev/buffer_dev.h"
#include "ppapi/cpp/instance_handle.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/var_array_buffer.h"

namespace pp {

class Instance;


class ContentDecryptor_Private {
 public:
  explicit ContentDecryptor_Private(Instance* instance);
  virtual ~ContentDecryptor_Private();

  
  
  
  
  
  virtual void Initialize(const std::string& key_system) = 0;
  virtual void CreateSession(uint32_t session_id,
                             const std::string& type,
                             pp::VarArrayBuffer init_data) = 0;
  virtual void UpdateSession(uint32_t session_id,
                             pp::VarArrayBuffer response) = 0;
  virtual void ReleaseSession(uint32_t session_id) = 0;
  virtual void Decrypt(pp::Buffer_Dev encrypted_buffer,
                       const PP_EncryptedBlockInfo& encrypted_block_info) = 0;
  virtual void InitializeAudioDecoder(
      const PP_AudioDecoderConfig& decoder_config,
      pp::Buffer_Dev extra_data_resource) = 0;
  virtual void InitializeVideoDecoder(
      const PP_VideoDecoderConfig& decoder_config,
      pp::Buffer_Dev extra_data_resource) = 0;
  virtual void DeinitializeDecoder(PP_DecryptorStreamType decoder_type,
                                   uint32_t request_id) = 0;
  virtual void ResetDecoder(PP_DecryptorStreamType decoder_type,
                            uint32_t request_id) = 0;
  
  virtual void DecryptAndDecode(
      PP_DecryptorStreamType decoder_type,
      pp::Buffer_Dev encrypted_buffer,
      const PP_EncryptedBlockInfo& encrypted_block_info) = 0;

  
  
  void SessionCreated(uint32_t session_id, const std::string& web_session_id);
  void SessionMessage(uint32_t session_id,
                      pp::VarArrayBuffer message,
                      const std::string& default_url);
  void SessionReady(uint32_t session_id);
  void SessionClosed(uint32_t session_id);
  void SessionError(uint32_t session_id,
                    int32_t media_error,
                    int32_t system_code);

  
  
  
  void DeliverBlock(pp::Buffer_Dev decrypted_block,
                    const PP_DecryptedBlockInfo& decrypted_block_info);

  void DecoderInitializeDone(PP_DecryptorStreamType decoder_type,
                             uint32_t request_id,
                             bool status);
  void DecoderDeinitializeDone(PP_DecryptorStreamType decoder_type,
                               uint32_t request_id);
  void DecoderResetDone(PP_DecryptorStreamType decoder_type,
                        uint32_t request_id);

  
  
  
  void DeliverFrame(pp::Buffer_Dev decrypted_frame,
                    const PP_DecryptedFrameInfo& decrypted_frame_info);

  
  
  
  void DeliverSamples(pp::Buffer_Dev audio_frames,
                      const PP_DecryptedSampleInfo& decrypted_sample_info);

 private:
  InstanceHandle associated_instance_;
};

}  

#endif  
