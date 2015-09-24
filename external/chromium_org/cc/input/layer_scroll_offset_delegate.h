// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_INPUT_LAYER_SCROLL_OFFSET_DELEGATE_H_
#define CC_INPUT_LAYER_SCROLL_OFFSET_DELEGATE_H_

#include "base/basictypes.h"
#include "ui/gfx/size_f.h"
#include "ui/gfx/vector2d_f.h"

namespace cc {

class LayerScrollOffsetDelegate {
 public:
  
  
  virtual void SetMaxScrollOffset(gfx::Vector2dF max_scroll_offset) = 0;

  
  
  virtual void SetTotalScrollOffset(gfx::Vector2dF new_value) = 0;

  
  
  
  
  
  
  
  virtual gfx::Vector2dF GetTotalScrollOffset() = 0;

  
  
  virtual bool IsExternalFlingActive() const = 0;

  
  
  virtual void SetTotalPageScaleFactor(float page_scale_factor) = 0;

  
  
  virtual void SetScrollableSize(gfx::SizeF scrollable_size) = 0;

 protected:
  LayerScrollOffsetDelegate() {}
  virtual ~LayerScrollOffsetDelegate() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(LayerScrollOffsetDelegate);
};

}  

#endif  
