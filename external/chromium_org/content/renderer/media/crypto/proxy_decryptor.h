// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_CRYPTO_PROXY_DECRYPTOR_H_
#define CONTENT_RENDERER_MEDIA_CRYPTO_PROXY_DECRYPTOR_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/synchronization/lock.h"
#include "media/base/decryptor.h"
#include "media/base/media_keys.h"

class GURL;

namespace blink {
#if defined(ENABLE_PEPPER_CDMS)
class WebFrame;
class WebMediaPlayerClient;
#endif  
}

namespace content {

#if defined(OS_ANDROID)
class RendererMediaPlayerManager;
#endif  

class ProxyDecryptor {
 public:
  
  
  typedef base::Callback<void(const std::string& session_id)> KeyAddedCB;
  typedef base::Callback<void(const std::string& session_id,
                              media::MediaKeys::KeyError error_code,
                              int system_code)> KeyErrorCB;
  typedef base::Callback<void(const std::string& session_id,
                              const std::vector<uint8>& message,
                              const std::string& default_url)> KeyMessageCB;

  ProxyDecryptor(
#if defined(ENABLE_PEPPER_CDMS)
      blink::WebMediaPlayerClient* web_media_player_client,
      blink::WebFrame* web_frame,
#elif defined(OS_ANDROID)
      RendererMediaPlayerManager* manager,
      int media_keys_id,
#endif  
      const KeyAddedCB& key_added_cb,
      const KeyErrorCB& key_error_cb,
      const KeyMessageCB& key_message_cb);
  virtual ~ProxyDecryptor();

  
  bool InitializeCDM(const std::string& key_system, const GURL& frame_url);

  
  
  
  
  void SetDecryptorReadyCB(const media::DecryptorReadyCB& decryptor_ready_cb);

  
  bool GenerateKeyRequest(const std::string& type,
                          const uint8* init_data,
                          int init_data_length);
  void AddKey(const uint8* key, int key_length,
              const uint8* init_data, int init_data_length,
              const std::string& session_id);
  void CancelKeyRequest(const std::string& session_id);

 private:
  
  typedef std::map<uint32, std::string> SessionIdMap;

  
  scoped_ptr<media::MediaKeys> CreateMediaKeys(const std::string& key_system,
                                               const GURL& frame_url);

  
  void OnSessionCreated(uint32 session_id, const std::string& web_session_id);
  void OnSessionMessage(uint32 session_id,
                        const std::vector<uint8>& message,
                        const std::string& default_url);
  void OnSessionReady(uint32 session_id);
  void OnSessionClosed(uint32 session_id);
  void OnSessionError(uint32 session_id,
                      media::MediaKeys::KeyError error_code,
                      int system_code);

  
  uint32 LookupSessionId(const std::string& web_session_id);

  
  
  
  const std::string& LookupWebSessionId(uint32 session_id);

  base::WeakPtrFactory<ProxyDecryptor> weak_ptr_factory_;

#if defined(ENABLE_PEPPER_CDMS)
  
  void DestroyHelperPlugin();

  
  blink::WebMediaPlayerClient* web_media_player_client_;
  blink::WebFrame* web_frame_;
#elif defined(OS_ANDROID)
  RendererMediaPlayerManager* manager_;
  int media_keys_id_;
#endif  

  
  
  scoped_ptr<media::MediaKeys> media_keys_;

  
  KeyAddedCB key_added_cb_;
  KeyErrorCB key_error_cb_;
  KeyMessageCB key_message_cb_;

  
  
  base::Lock lock_;

  media::DecryptorReadyCB decryptor_ready_cb_;

  
  
  
  static uint32 next_session_id_;

  SessionIdMap sessions_;

  bool is_clear_key_;

  DISALLOW_COPY_AND_ASSIGN(ProxyDecryptor);
};

}  

#endif  
