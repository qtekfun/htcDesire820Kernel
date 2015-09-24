// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_FRAME_HOST_NAVIGATOR_H_
#define CONTENT_BROWSER_FRAME_HOST_NAVIGATOR_H_

#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"

class GURL;

namespace content {

class NavigationControllerImpl;
class NavigatorDelegate;
class RenderFrameHostImpl;

class CONTENT_EXPORT Navigator : public base::RefCounted<Navigator> {
 public:
  
  virtual void DidStartProvisionalLoad(RenderFrameHostImpl* render_frame_host,
                                       int64 frame_id,
                                       int64 parent_frame_id,
                                       bool main_frame,
                                       const GURL& url) {};

 protected:
  friend class base::RefCounted<Navigator>;
  virtual ~Navigator() {}
};

}  

#endif  
