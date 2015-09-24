// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_UTILITY_PROCESS_HOST_CLIENT_H_
#define CONTENT_PUBLIC_BROWSER_UTILITY_PROCESS_HOST_CLIENT_H_

#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"

namespace IPC {
class Message;
}

namespace content {

class UtilityProcessHostClient
    : public base::RefCountedThreadSafe<UtilityProcessHostClient> {
 public:
  
  virtual void OnProcessCrashed(int exit_code) {}

  
  virtual bool OnMessageReceived(const IPC::Message& message) = 0;

 protected:
  friend class base::RefCountedThreadSafe<UtilityProcessHostClient>;

  virtual ~UtilityProcessHostClient() {}
};

};  

#endif  
