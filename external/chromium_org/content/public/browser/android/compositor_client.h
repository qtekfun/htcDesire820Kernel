// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_ANDROID_COMPOSITOR_CLIENT_H_
#define CONTENT_PUBLIC_BROWSER_ANDROID_COMPOSITOR_CLIENT_H_

#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT CompositorClient {
 public:
  
  virtual void ScheduleComposite() = 0;

  
  virtual void OnSwapBuffersCompleted() {}

  
  virtual void OnSwapBuffersPosted() {}

  
  virtual void DidLoseResources() {}

  
  virtual void DidLoseUIResources() {}

  
  virtual void UIResourcesAreInvalid() {}

 protected:
  CompositorClient() {}
  virtual ~CompositorClient() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(CompositorClient);
};

}  

#endif  
