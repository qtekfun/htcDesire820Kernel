// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_FRAME_HOST_NAVIGATOR_IMPL_H_
#define CONTENT_BROWSER_FRAME_HOST_NAVIGATOR_IMPL_H_

#include "base/memory/ref_counted.h"
#include "content/browser/frame_host/navigator.h"
#include "content/common/content_export.h"

namespace content {

class NavigationControllerImpl;
class NavigatorDelegate;

class CONTENT_EXPORT NavigatorImpl : public Navigator {
 public:
  NavigatorImpl(NavigationControllerImpl* navigation_controller,
                NavigatorDelegate* delegate);

  
  virtual void DidStartProvisionalLoad(RenderFrameHostImpl* render_frame_host,
                                       int64 frame_id,
                                       int64 parent_frame_id,
                                       bool main_frame,
                                       const GURL& url) OVERRIDE;

 private:
  virtual ~NavigatorImpl() {}

  
  
  
  
  NavigationControllerImpl* controller_;

  
  
  NavigatorDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(NavigatorImpl);
};

}  

#endif  
