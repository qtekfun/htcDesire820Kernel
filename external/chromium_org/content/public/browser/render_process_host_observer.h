// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_RENDER_PROCESS_HOST_OBSERVER_H_
#define CONTENT_PUBLIC_BROWSER_RENDER_PROCESS_HOST_OBSERVER_H_

#include "content/common/content_export.h"

namespace content {

class RenderProcessHost;

class CONTENT_EXPORT RenderProcessHostObserver {
 public:
  
  virtual void RenderProcessHostDestroyed(RenderProcessHost* host) {}

 protected:
  virtual ~RenderProcessHostObserver() {}
};

}  

#endif  
