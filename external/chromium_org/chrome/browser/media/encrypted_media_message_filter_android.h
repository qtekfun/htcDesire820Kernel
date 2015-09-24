// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_ENCRYPTED_MEDIA_MESSAGE_FILTER_ANDROID_H_
#define CHROME_BROWSER_MEDIA_ENCRYPTED_MEDIA_MESSAGE_FILTER_ANDROID_H_

#include "base/basictypes.h"
#include "content/public/browser/browser_message_filter.h"

struct SupportedKeySystemRequest;
struct SupportedKeySystemResponse;

namespace chrome {

class EncryptedMediaMessageFilterAndroid
    : public content::BrowserMessageFilter {
 public:
  EncryptedMediaMessageFilterAndroid();

 private:
  virtual ~EncryptedMediaMessageFilterAndroid();

  
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;
  virtual void OverrideThreadForMessage(
      const IPC::Message& message,
      content::BrowserThread::ID* thread) OVERRIDE;

  
  void OnGetSupportedKeySystems(
      const SupportedKeySystemRequest& request,
      SupportedKeySystemResponse* response);

  DISALLOW_COPY_AND_ASSIGN(EncryptedMediaMessageFilterAndroid);
};

}  

#endif  
