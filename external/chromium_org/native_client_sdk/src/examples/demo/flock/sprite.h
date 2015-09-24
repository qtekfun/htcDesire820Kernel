// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SPRITE_H_
#define SPRITE_H_

#include <vector>
#include "ppapi/cpp/point.h"
#include "ppapi/cpp/rect.h"
#include "ppapi/cpp/size.h"


class Sprite {
 public:
  
  
  
  
  
  Sprite(uint32_t* pixel_buffer, const pp::Size& size, int32_t stride = 0);

  
  
  ~Sprite();

  
  
  
  
  void SetPixelBuffer(uint32_t* pixel_buffer,
                      const pp::Size& size,
                      int32_t row_bytes);

  
  
  
  void CompositeFromRectToPoint(const pp::Rect& src_rect,
                                uint32_t* dest_pixel_buffer,
                                const pp::Rect& dest_bounds,
                                int32_t dest_row_bytes,
                                const pp::Point& dest_point) const;

  
  const pp::Size& size() const {
    return pixel_buffer_size_;
  }

 private:
  uint32_t* pixel_buffer_;
  pp::Size pixel_buffer_size_;
  int32_t row_bytes_;
};

#endif  
