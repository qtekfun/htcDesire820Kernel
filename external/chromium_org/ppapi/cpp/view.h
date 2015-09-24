// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_VIEW_H_
#define PPAPI_CPP_VIEW_H_

#include "ppapi/cpp/resource.h"
#include "ppapi/cpp/rect.h"
#include "ppapi/cpp/size.h"


namespace pp {

class View : public Resource {
 public:
  
  View();

  
  
  View(PP_Resource view_resource);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  Rect GetRect() const;

  
  
  
  
  
  bool IsFullscreen() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool IsVisible() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  bool IsPageVisible() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  Rect GetClipRect() const;

  
  
  
  
  
  
  
  
  
  
  float GetDeviceScale() const;

  
  
  
  
  
  
  float GetCSSScale() const;
};

}  

#endif  
