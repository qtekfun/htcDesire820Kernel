// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CRYPTO_AES_DECRYPTOR_H_
#define MEDIA_CRYPTO_AES_DECRYPTOR_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/containers/scoped_ptr_hash_map.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "media/base/decryptor.h"
#include "media/base/media_export.h"
#include "media/base/media_keys.h"

namespace crypto {
class SymmetricKey;
}

namespace media {

class MEDIA_EXPORT AesDecryptor : public MediaKeys, public Decryptor {
 public:
  AesDecryptor(const SessionCreatedCB& session_created_cb,
               const SessionMessageCB& session_message_cb,
               const SessionReadyCB& session_ready_cb,
               const SessionClosedCB& session_closed_cb,
               const SessionErrorCB& session_error_cb);
  virtual ~AesDecryptor();

  
  virtual bool CreateSession(uint32 session_id,
                             const std::string& type,
                             const uint8* init_data,
                             int init_data_length) OVERRIDE;
  virtual void UpdateSession(uint32 session_id,
                             const uint8* response,
                             int response_length) OVERRIDE;
  virtual void ReleaseSession(uint32 session_id) OVERRIDE;
  virtual Decryptor* GetDecryptor() OVERRIDE;

  
  virtual void RegisterNewKeyCB(StreamType stream_type,
                                const NewKeyCB& key_added_cb) OVERRIDE;
  virtual void Decrypt(StreamType stream_type,
                       const scoped_refptr<DecoderBuffer>& encrypted,
                       const DecryptCB& decrypt_cb) OVERRIDE;
  virtual void CancelDecrypt(StreamType stream_type) OVERRIDE;
  virtual void InitializeAudioDecoder(const AudioDecoderConfig& config,
                                      const DecoderInitCB& init_cb) OVERRIDE;
  virtual void InitializeVideoDecoder(const VideoDecoderConfig& config,
                                      const DecoderInitCB& init_cb) OVERRIDE;
  virtual void DecryptAndDecodeAudio(
      const scoped_refptr<DecoderBuffer>& encrypted,
      const AudioDecodeCB& audio_decode_cb) OVERRIDE;
  virtual void DecryptAndDecodeVideo(
      const scoped_refptr<DecoderBuffer>& encrypted,
      const VideoDecodeCB& video_decode_cb) OVERRIDE;
  virtual void ResetDecoder(StreamType stream_type) OVERRIDE;
  virtual void DeinitializeDecoder(StreamType stream_type) OVERRIDE;

 private:
  
  
  
  class DecryptionKey {
   public:
    explicit DecryptionKey(const std::string& secret);
    ~DecryptionKey();

    
    bool Init();

    crypto::SymmetricKey* decryption_key() { return decryption_key_.get(); }

   private:
    
    const std::string secret_;

    
    scoped_ptr<crypto::SymmetricKey> decryption_key_;

    DISALLOW_COPY_AND_ASSIGN(DecryptionKey);
  };

  
  
  
  
  class SessionIdDecryptionKeyMap;

  
  typedef base::ScopedPtrHashMap<std::string, SessionIdDecryptionKeyMap>
      KeyIdToSessionKeysMap;

  
  
  bool AddDecryptionKey(const uint32 session_id,
                        const std::string& key_id,
                        const std::string& key_string);

  
  
  DecryptionKey* GetKey(const std::string& key_id) const;

  
  void DeleteKeysForSession(const uint32 session_id);

  
  SessionCreatedCB session_created_cb_;
  SessionMessageCB session_message_cb_;
  SessionReadyCB session_ready_cb_;
  SessionClosedCB session_closed_cb_;
  SessionErrorCB session_error_cb_;

  
  
  
  KeyIdToSessionKeysMap key_map_;  
  mutable base::Lock key_map_lock_;  

  
  std::set<uint32> valid_sessions_;

  
  
  static uint32 next_web_session_id_;

  NewKeyCB new_audio_key_cb_;
  NewKeyCB new_video_key_cb_;

  DISALLOW_COPY_AND_ASSIGN(AesDecryptor);
};

}  

#endif  
