// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_ZYGOTE_HOST_LINUX_H_
#define CONTENT_PUBLIC_BROWSER_ZYGOTE_HOST_LINUX_H_

#include <unistd.h>

#include "base/process/process.h"
#include "content/common/content_export.h"

namespace content {


class ZygoteHost {
 public:
  
  CONTENT_EXPORT static ZygoteHost* GetInstance();

  virtual ~ZygoteHost() {}

  
  virtual pid_t GetPid() const = 0;

  
  virtual pid_t GetSandboxHelperPid() const = 0;

  
  
  virtual int GetSandboxStatus() const = 0;

  
  
  
  virtual void AdjustRendererOOMScore(base::ProcessHandle process_handle,
                                      int score) = 0;
};

}  

#endif  
