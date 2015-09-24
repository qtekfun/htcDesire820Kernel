// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WINDOW_WINDOW_RESOURCES_H_
#define UI_VIEWS_WINDOW_WINDOW_RESOURCES_H_

namespace gfx {
class ImageSkia;
}

namespace views {

typedef int FramePartImage;

class WindowResources {
 public:
  virtual ~WindowResources() {}

  virtual gfx::ImageSkia* GetPartImage(FramePartImage part) const = 0;
};

}  

#endif  
