// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_BROWSER_MAIN_PARTS_H_
#define CONTENT_PUBLIC_BROWSER_BROWSER_MAIN_PARTS_H_

#include "base/basictypes.h"
#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT BrowserMainParts {
 public:
  BrowserMainParts() {}
  virtual ~BrowserMainParts() {}

  virtual void PreEarlyInitialization() {}

  virtual void PostEarlyInitialization() {}

  virtual void PreMainMessageLoopStart() {}

  virtual void PostMainMessageLoopStart() {}

  
  virtual void ToolkitInitialized() {}

  
  
  
  
  
  
  virtual int PreCreateThreads();

  
  
  virtual void PreMainMessageLoopRun() {}

  
  
  
  virtual bool MainMessageLoopRun(int* result_code);

  
  
  virtual void PostMainMessageLoopRun() {}

  
  
  virtual void PostDestroyThreads() {}
};

}  

#endif  
