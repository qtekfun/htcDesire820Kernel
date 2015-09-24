// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_CHILD_PROCESS_HOST_DELEGATE_H_
#define CONTENT_PUBLIC_COMMON_CHILD_PROCESS_HOST_DELEGATE_H_

#include <string>

#include "content/common/content_export.h"
#include "ipc/ipc_listener.h"

namespace content {

class ChildProcessHostDelegate : public IPC::Listener {
 public:
  virtual ~ChildProcessHostDelegate() {}

  
  
  
  
  CONTENT_EXPORT virtual bool CanShutdown();

  
  
  virtual void OnChildDisconnected() {}
};

};  

#endif  
