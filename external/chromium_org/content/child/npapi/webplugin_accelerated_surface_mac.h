// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_NPAPI_WEBPLUGIN_ACCELERATED_SURFACE_MAC_H_
#define CONTENT_CHILD_NPAPI_WEBPLUGIN_ACCELERATED_SURFACE_MAC_H_

#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"

typedef struct _CGLContextObject* CGLContextObj;

namespace content {

class WebPluginAcceleratedSurface {
 public:
  virtual ~WebPluginAcceleratedSurface() {}

  
  virtual void SetSize(const gfx::Size& size) = 0;

  
  
  virtual CGLContextObj context() = 0;

  
  virtual void StartDrawing() = 0;

  
  
  virtual void EndDrawing() = 0;
};

}  

#endif  
