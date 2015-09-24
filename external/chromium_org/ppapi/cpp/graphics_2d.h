// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_GRAPHICS_2D_H_
#define PPAPI_CPP_GRAPHICS_2D_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/cpp/resource.h"
#include "ppapi/cpp/size.h"


namespace pp {

class CompletionCallback;
class ImageData;
class InstanceHandle;
class Point;
class Rect;

class Graphics2D : public Resource {
 public:
  
  
  Graphics2D();

  
  
  
  
  
  Graphics2D(const Graphics2D& other);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  Graphics2D(const InstanceHandle& instance,
             const Size& size,
             bool is_always_opaque);

  
  
  
  
  virtual ~Graphics2D();

  
  
  
  
  
  
  
  
  Graphics2D& operator=(const Graphics2D& other);

  
  
  
  const Size& size() const { return size_; }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void PaintImageData(const ImageData& image,
                      const Point& top_left);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void PaintImageData(const ImageData& image,
                      const Point& top_left,
                      const Rect& src_rect);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void Scroll(const Rect& clip, const Point& amount);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void ReplaceContents(ImageData* image);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  

  
  
  int32_t Flush(const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool SetScale(float scale);

  
  
  
  
  
  
  float GetScale();

 private:
  Size size_;
};

}  

#endif  
