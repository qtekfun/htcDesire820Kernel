// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_MOUSE_CURSOR_H_
#define PPAPI_CPP_MOUSE_CURSOR_H_

#include "ppapi/c/ppb_mouse_cursor.h"
#include "ppapi/cpp/image_data.h"
#include "ppapi/cpp/instance_handle.h"
#include "ppapi/cpp/point.h"

namespace pp {

class MouseCursor {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool SetCursor(const InstanceHandle& instance,
                        PP_MouseCursor_Type type,
                        const ImageData& image = ImageData(),
                        const Point& hot_spot = Point(0, 0));
};

}  

#endif  
