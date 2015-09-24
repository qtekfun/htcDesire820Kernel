// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_BROWSER_CHILD_PROCESS_HOST_DELEGATE_H_
#define CONTENT_PUBLIC_BROWSER_BROWSER_CHILD_PROCESS_HOST_DELEGATE_H_

#include "content/common/content_export.h"
#include "ipc/ipc_listener.h"

namespace content {

class CONTENT_EXPORT BrowserChildProcessHostDelegate : public IPC::Listener {
 public:
  virtual ~BrowserChildProcessHostDelegate() {}

  
  
  
  
  virtual bool CanShutdown();

  
  virtual void OnProcessLaunched() {}

  
  
  
  virtual void OnProcessCrashed(int exit_code) {}
};

};  

#endif  
