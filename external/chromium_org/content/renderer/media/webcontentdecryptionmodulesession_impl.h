// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_WEBCONTENTDECRYPTIONMODULESESSION_IMPL_H_
#define CONTENT_RENDERER_MEDIA_WEBCONTENTDECRYPTIONMODULESESSION_IMPL_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "media/base/media_keys.h"
#include "third_party/WebKit/public/platform/WebContentDecryptionModuleSession.h"
#include "third_party/WebKit/public/platform/WebString.h"

namespace media {
class MediaKeys;
}

namespace content {

class WebContentDecryptionModuleSessionImpl
    : public blink::WebContentDecryptionModuleSession {
 public:
  typedef base::Callback<void(uint32 session_id)> SessionClosedCB;

  WebContentDecryptionModuleSessionImpl(
      uint32 session_id,
      media::MediaKeys* media_keys,
      Client* client,
      const SessionClosedCB& session_closed_cb);
  virtual ~WebContentDecryptionModuleSessionImpl();

  
  virtual blink::WebString sessionId() const OVERRIDE;
  virtual void generateKeyRequest(const blink::WebString& mime_type,
                                  const uint8* init_data,
                                  size_t init_data_length) OVERRIDE;
  virtual void update(const uint8* response, size_t response_length) OVERRIDE;
  virtual void close() OVERRIDE;

  
  void OnSessionCreated(const std::string& web_session_id);
  void OnSessionMessage(const std::vector<uint8>& message,
                        const std::string& destination_url);
  void OnSessionReady();
  void OnSessionClosed();
  void OnSessionError(media::MediaKeys::KeyError error_code, int system_code);

 private:
  
  media::MediaKeys* media_keys_;
  Client* client_;

  SessionClosedCB session_closed_cb_;

  
  
  blink::WebString web_session_id_;

  
  
  const uint32 session_id_;

  DISALLOW_COPY_AND_ASSIGN(WebContentDecryptionModuleSessionImpl);
};

}  

#endif  
