// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_MEDIA_KEYS_H_
#define MEDIA_BASE_MEDIA_KEYS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "media/base/media_export.h"

namespace media {

class Decryptor;

class MEDIA_EXPORT MediaKeys {
 public:
  
  
  
  enum KeyError {
    kUnknownError = 1,
    kClientError,
    
    
    kOutputError = 4,
    
    
    kMaxKeyError  
  };

  const static uint32 kInvalidSessionId = 0;

  MediaKeys();
  virtual ~MediaKeys();

  
  
  
  
  
  virtual bool CreateSession(uint32 session_id,
                             const std::string& type,
                             const uint8* init_data,
                             int init_data_length) = 0;

  
  virtual void UpdateSession(uint32 session_id,
                             const uint8* response,
                             int response_length) = 0;

  
  virtual void ReleaseSession(uint32 session_id) = 0;

  
  
  
  virtual Decryptor* GetDecryptor();

 private:
  DISALLOW_COPY_AND_ASSIGN(MediaKeys);
};

typedef base::Callback<
    void(uint32 session_id, const std::string& web_session_id)>
    SessionCreatedCB;

typedef base::Callback<void(uint32 session_id,
                            const std::vector<uint8>& message,
                            const std::string& destination_url)>
    SessionMessageCB;

typedef base::Callback<void(uint32 session_id)> SessionReadyCB;

typedef base::Callback<void(uint32 session_id)> SessionClosedCB;

typedef base::Callback<void(uint32 session_id,
                            media::MediaKeys::KeyError error_code,
                            int system_code)> SessionErrorCB;

}  

#endif  
