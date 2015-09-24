// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_GRAPHICS_2D_DEV_H_
#define PPAPI_CPP_DEV_GRAPHICS_2D_DEV_H_

#include "ppapi/cpp/graphics_2d.h"

#include "ppapi/c/dev/ppb_graphics_2d_dev.h"

namespace pp {

class Point;

class Graphics2D_Dev : public Graphics2D {
 public:
  
  
  Graphics2D_Dev() : Graphics2D() {}

  
  
  Graphics2D_Dev(const Graphics2D& other) : Graphics2D(other) {}

  virtual ~Graphics2D_Dev() {}

  
  static bool SupportsScale();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool SetScale(float scale);

  
  
  
  
  
  float GetScale();

  
  
  
  
  
  
  void SetOffset(const pp::Point& offset);

  
  
  
  
  
  
  
  void SetResizeMode(PP_Graphics2D_Dev_ResizeMode resize_mode);
};

}  

#endif  
