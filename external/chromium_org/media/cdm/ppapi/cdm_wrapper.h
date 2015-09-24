// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CDM_PPAPI_CDM_WRAPPER_H_
#define MEDIA_CDM_PPAPI_CDM_WRAPPER_H_

#include <map>
#include <queue>
#include <string>

#include "base/basictypes.h"
#include "media/cdm/ppapi/api/content_decryption_module.h"
#include "media/cdm/ppapi/cdm_helpers.h"
#include "media/cdm/ppapi/supported_cdm_versions.h"
#include "ppapi/cpp/logging.h"

namespace media {

class CdmWrapper {
 public:
  
  
  
  
  enum Result {
    NO_ACTION,
    CALL_KEY_ADDED,
    CALL_KEY_ERROR
  };

  static CdmWrapper* Create(const char* key_system,
                            uint32_t key_system_size,
                            GetCdmHostFunc get_cdm_host_func,
                            void* user_data);

  virtual ~CdmWrapper() {};

  virtual void CreateSession(uint32_t session_id,
                             const char* type,
                             uint32_t type_size,
                             const uint8_t* init_data,
                             uint32_t init_data_size) = 0;
  virtual Result UpdateSession(uint32_t session_id,
                               const uint8_t* response,
                               uint32_t response_size) = 0;
  virtual Result ReleaseSession(uint32_t session_id) = 0;
  virtual void TimerExpired(void* context) = 0;
  virtual cdm::Status Decrypt(const cdm::InputBuffer& encrypted_buffer,
                              cdm::DecryptedBlock* decrypted_buffer) = 0;
  virtual cdm::Status InitializeAudioDecoder(
      const cdm::AudioDecoderConfig& audio_decoder_config) = 0;
  virtual cdm::Status InitializeVideoDecoder(
      const cdm::VideoDecoderConfig& video_decoder_config) = 0;
  virtual void DeinitializeDecoder(cdm::StreamType decoder_type) = 0;
  virtual void ResetDecoder(cdm::StreamType decoder_type) = 0;
  virtual cdm::Status DecryptAndDecodeFrame(
      const cdm::InputBuffer& encrypted_buffer,
      cdm::VideoFrame* video_frame) = 0;
  virtual cdm::Status DecryptAndDecodeSamples(
      const cdm::InputBuffer& encrypted_buffer,
      cdm::AudioFrames* audio_frames) = 0;
  virtual void OnPlatformChallengeResponse(
      const cdm::PlatformChallengeResponse& response) = 0;
  virtual void OnQueryOutputProtectionStatus(
      uint32_t link_mask,
      uint32_t output_protection_mask) = 0;

  
  
  
  
  
  
  
  
  

  
  virtual uint32_t LookupSessionId(const std::string& web_session_id) = 0;

  
  virtual const std::string LookupWebSessionId(uint32_t session_id) = 0;

  
  
  
  typedef std::map<uint32_t, std::string> SessionMap;
  SessionMap session_map_;

  static const uint32_t kInvalidSessionId = 0;

  
  
  
  
  
  
  
  uint32_t current_key_request_session_id_;
  std::queue<uint32_t> pending_key_request_session_ids_;

 protected:
  CdmWrapper() : current_key_request_session_id_(kInvalidSessionId) {}

 private:
  DISALLOW_COPY_AND_ASSIGN(CdmWrapper);
};

template <class CdmInterface>
class CdmWrapperImpl : public CdmWrapper {
 public:
  static CdmWrapper* Create(const char* key_system,
                            uint32_t key_system_size,
                            GetCdmHostFunc get_cdm_host_func,
                            void* user_data) {
    void* cdm_instance = ::CreateCdmInstance(
        CdmInterface::kVersion, key_system, key_system_size, get_cdm_host_func,
        user_data);
    if (!cdm_instance)
      return NULL;

    return new CdmWrapperImpl<CdmInterface>(
        static_cast<CdmInterface*>(cdm_instance));
  }

  virtual ~CdmWrapperImpl() {
    cdm_->Destroy();
  }

  virtual void CreateSession(uint32_t session_id,
                             const char* type,
                             uint32_t type_size,
                             const uint8_t* init_data,
                             uint32_t init_data_size) OVERRIDE {
    cdm_->CreateSession(session_id, type, type_size, init_data, init_data_size);
  }

  virtual Result UpdateSession(uint32_t session_id,
                               const uint8_t* response,
                               uint32_t response_size) OVERRIDE {
    cdm_->UpdateSession(session_id, response, response_size);
    return NO_ACTION;
  }

  virtual Result ReleaseSession(uint32_t session_id) OVERRIDE {
    cdm_->ReleaseSession(session_id);
    return NO_ACTION;
  }

  virtual void TimerExpired(void* context) OVERRIDE {
    cdm_->TimerExpired(context);
  }

  virtual cdm::Status Decrypt(const cdm::InputBuffer& encrypted_buffer,
                              cdm::DecryptedBlock* decrypted_buffer) OVERRIDE {
    return cdm_->Decrypt(encrypted_buffer, decrypted_buffer);
  }

  virtual cdm::Status InitializeAudioDecoder(
      const cdm::AudioDecoderConfig& audio_decoder_config) OVERRIDE {
    return cdm_->InitializeAudioDecoder(audio_decoder_config);
  }

  virtual cdm::Status InitializeVideoDecoder(
      const cdm::VideoDecoderConfig& video_decoder_config) OVERRIDE {
    return cdm_->InitializeVideoDecoder(video_decoder_config);
  }

  virtual void DeinitializeDecoder(cdm::StreamType decoder_type) OVERRIDE {
    cdm_->DeinitializeDecoder(decoder_type);
  }

  virtual void ResetDecoder(cdm::StreamType decoder_type) OVERRIDE {
    cdm_->ResetDecoder(decoder_type);
  }

  virtual cdm::Status DecryptAndDecodeFrame(
      const cdm::InputBuffer& encrypted_buffer,
      cdm::VideoFrame* video_frame) OVERRIDE {
    return cdm_->DecryptAndDecodeFrame(encrypted_buffer, video_frame);
  }

  virtual cdm::Status DecryptAndDecodeSamples(
      const cdm::InputBuffer& encrypted_buffer,
      cdm::AudioFrames* audio_frames) OVERRIDE {
    return cdm_->DecryptAndDecodeSamples(encrypted_buffer, audio_frames);
  }

  virtual void OnPlatformChallengeResponse(
      const cdm::PlatformChallengeResponse& response) OVERRIDE {
    cdm_->OnPlatformChallengeResponse(response);
  }

  virtual void OnQueryOutputProtectionStatus(
      uint32_t link_mask,
      uint32_t output_protection_mask) OVERRIDE {
    cdm_->OnQueryOutputProtectionStatus(link_mask, output_protection_mask);
  }

  uint32_t LookupSessionId(const std::string& web_session_id) {
    for (SessionMap::iterator it = session_map_.begin();
         it != session_map_.end();
         ++it) {
      if (it->second == web_session_id)
        return it->first;
    }

    
    
    
    uint32_t session_id = current_key_request_session_id_;
    if (current_key_request_session_id_) {
      
      
      current_key_request_session_id_ = kInvalidSessionId;
    } else {
      PP_DCHECK(!pending_key_request_session_ids_.empty());
      session_id = pending_key_request_session_ids_.front();
      pending_key_request_session_ids_.pop();
    }

    
    
    
    if (!web_session_id.empty()) {
      PP_DCHECK(session_map_.find(session_id) == session_map_.end());
      session_map_[session_id] = web_session_id;
    }

    return session_id;
  }

  const std::string LookupWebSessionId(uint32_t session_id) {
    
    SessionMap::iterator it = session_map_.find(session_id);
    return (it != session_map_.end()) ? it->second : std::string();
  }

 private:
  CdmWrapperImpl(CdmInterface* cdm) : cdm_(cdm) {
    PP_DCHECK(cdm_);
  }

  CdmInterface* cdm_;

  DISALLOW_COPY_AND_ASSIGN(CdmWrapperImpl);
};


template <class CdmInterface>
void PrefixedGenerateKeyRequest(CdmWrapper* wrapper,
                                CdmInterface* cdm,
                                uint32_t session_id,
                                const char* type,
                                uint32_t type_size,
                                const uint8_t* init_data,
                                uint32_t init_data_size) {
  
  
  wrapper->current_key_request_session_id_ = session_id;

  cdm::Status status =
      cdm->GenerateKeyRequest(type, type_size, init_data, init_data_size);
  PP_DCHECK(status == cdm::kSuccess || status == cdm::kSessionError);
  if (status != cdm::kSuccess) {
    
    
    PP_DCHECK(wrapper->current_key_request_session_id_ ==
              CdmWrapper::kInvalidSessionId);
    wrapper->current_key_request_session_id_ = CdmWrapper::kInvalidSessionId;
    return;
  }

  if (wrapper->current_key_request_session_id_) {
    
    
    
    
    
    
    wrapper->pending_key_request_session_ids_.push(session_id);
    wrapper->current_key_request_session_id_ = CdmWrapper::kInvalidSessionId;
  }
}

template <class CdmInterface>
CdmWrapper::Result PrefixedAddKey(CdmWrapper* wrapper,
                                  CdmInterface* cdm,
                                  uint32_t session_id,
                                  const uint8_t* response,
                                  uint32_t response_size) {
  const std::string web_session_id = wrapper->LookupWebSessionId(session_id);
  if (web_session_id.empty()) {
    
    return CdmWrapper::CALL_KEY_ERROR;
  }

  
  
  cdm::Status status = cdm->AddKey(web_session_id.data(), web_session_id.size(),
                                   response, response_size,
                                   NULL, 0);
  PP_DCHECK(status == cdm::kSuccess || status == cdm::kSessionError);
  if (status != cdm::kSuccess) {
    
    return CdmWrapper::CALL_KEY_ERROR;
  }

  return CdmWrapper::CALL_KEY_ADDED;
}

template <class CdmInterface>
CdmWrapper::Result PrefixedCancelKeyRequest(CdmWrapper* wrapper,
                                            CdmInterface* cdm,
                                            uint32_t session_id) {
  const std::string web_session_id = wrapper->LookupWebSessionId(session_id);
  if (web_session_id.empty()) {
    
    return CdmWrapper::CALL_KEY_ERROR;
  }

  wrapper->session_map_.erase(session_id);
  cdm::Status status =
      cdm->CancelKeyRequest(web_session_id.data(), web_session_id.size());

  PP_DCHECK(status == cdm::kSuccess || status == cdm::kSessionError);
  if (status != cdm::kSuccess) {
    
    return CdmWrapper::CALL_KEY_ERROR;
  }

  return CdmWrapper::NO_ACTION;
}


template <>
void CdmWrapperImpl<cdm::ContentDecryptionModule_1>::CreateSession(
    uint32_t session_id,
    const char* type,
    uint32_t type_size,
    const uint8_t* init_data,
    uint32_t init_data_size) {
  PrefixedGenerateKeyRequest(
      this, cdm_, session_id, type, type_size, init_data, init_data_size);
}

template <>
CdmWrapper::Result CdmWrapperImpl<
    cdm::ContentDecryptionModule_1>::UpdateSession(uint32_t session_id,
                                                   const uint8_t* response,
                                                   uint32_t response_size) {
  return PrefixedAddKey(this, cdm_, session_id, response, response_size);
}

template <>
CdmWrapper::Result CdmWrapperImpl<
    cdm::ContentDecryptionModule_1>::ReleaseSession(uint32_t session_id) {
  return PrefixedCancelKeyRequest(this, cdm_, session_id);
}

template <> void CdmWrapperImpl<cdm::ContentDecryptionModule_1>::
    OnPlatformChallengeResponse(
        const cdm::PlatformChallengeResponse& response) {
  PP_NOTREACHED();
}

template <> void CdmWrapperImpl<cdm::ContentDecryptionModule_1>::
    OnQueryOutputProtectionStatus(uint32_t link_mask,
                                  uint32_t output_protection_mask) {
  PP_NOTREACHED();
}

template <> cdm::Status CdmWrapperImpl<cdm::ContentDecryptionModule_1>::
    DecryptAndDecodeSamples(const cdm::InputBuffer& encrypted_buffer,
                            cdm::AudioFrames* audio_frames) {
  AudioFramesImpl audio_frames_1;
  cdm::Status status =
      cdm_->DecryptAndDecodeSamples(encrypted_buffer, &audio_frames_1);
  if (status != cdm::kSuccess)
    return status;

  audio_frames->SetFrameBuffer(audio_frames_1.PassFrameBuffer());
  audio_frames->SetFormat(cdm::kAudioFormatS16);
  return cdm::kSuccess;
}


template <>
void CdmWrapperImpl<cdm::ContentDecryptionModule_2>::CreateSession(
    uint32_t session_id,
    const char* type,
    uint32_t type_size,
    const uint8_t* init_data,
    uint32_t init_data_size) {
  PrefixedGenerateKeyRequest(
      this, cdm_, session_id, type, type_size, init_data, init_data_size);
}

template <>
CdmWrapper::Result CdmWrapperImpl<
    cdm::ContentDecryptionModule_2>::UpdateSession(uint32_t session_id,
                                                   const uint8_t* response,
                                                   uint32_t response_size) {
  return PrefixedAddKey(this, cdm_, session_id, response, response_size);
}

template <>
CdmWrapper::Result CdmWrapperImpl<
    cdm::ContentDecryptionModule_2>::ReleaseSession(uint32_t session_id) {
  return PrefixedCancelKeyRequest(this, cdm_, session_id);
}

CdmWrapper* CdmWrapper::Create(const char* key_system,
                               uint32_t key_system_size,
                               GetCdmHostFunc get_cdm_host_func,
                               void* user_data) {
  COMPILE_ASSERT(cdm::ContentDecryptionModule::kVersion ==
                 cdm::ContentDecryptionModule_3::kVersion,
                 update_code_below);

  
  
  
  
  PP_DCHECK(
      !IsSupportedCdmInterfaceVersion(
          cdm::ContentDecryptionModule::kVersion + 1) &&
      IsSupportedCdmInterfaceVersion(cdm::ContentDecryptionModule::kVersion) &&
      IsSupportedCdmInterfaceVersion(
          cdm::ContentDecryptionModule_2::kVersion) &&
      IsSupportedCdmInterfaceVersion(
          cdm::ContentDecryptionModule_1::kVersion) &&
      !IsSupportedCdmInterfaceVersion(
          cdm::ContentDecryptionModule_1::kVersion - 1));

  
  CdmWrapper* cdm_wrapper =
      CdmWrapperImpl<cdm::ContentDecryptionModule>::Create(
          key_system, key_system_size, get_cdm_host_func, user_data);
  if (cdm_wrapper)
    return cdm_wrapper;

  
  cdm_wrapper = CdmWrapperImpl<cdm::ContentDecryptionModule_2>::Create(
      key_system, key_system_size, get_cdm_host_func, user_data);
  if (cdm_wrapper)
    return cdm_wrapper;

  cdm_wrapper = CdmWrapperImpl<cdm::ContentDecryptionModule_1>::Create(
      key_system, key_system_size, get_cdm_host_func, user_data);
  return cdm_wrapper;
}

COMPILE_ASSERT(cdm::ContentDecryptionModule::kVersion ==
                   cdm::ContentDecryptionModule_3::kVersion,
               ensure_cdm_wrapper_templates_have_old_version_support);

}  

#endif  
