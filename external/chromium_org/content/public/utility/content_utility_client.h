// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_UTILITY_CONTENT_UTILITY_CLIENT_H_
#define CONTENT_PUBLIC_UTILITY_CONTENT_UTILITY_CLIENT_H_

#include "content/public/common/content_client.h"

namespace content {

class CONTENT_EXPORT ContentUtilityClient {
 public:
  
  virtual void UtilityThreadStarted() {}

  
  virtual bool OnMessageReceived(const IPC::Message& message);
};

}  

#endif  
