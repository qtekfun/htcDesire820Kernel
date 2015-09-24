// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_V2_PUBLIC_LAYOUT_H_
#define UI_V2_PUBLIC_LAYOUT_H_

#include "ui/v2/public/v2_export.h"
#include "ui/v2/public/view_observer.h"

namespace v2 {

class V2_EXPORT Layout {
 public:
  virtual ~Layout();

  
  
  virtual void SetChildBounds(View* child,
                              const gfx::Rect& requested_bounds);

 protected:
  
  void SetChildBoundsDirect(View* child, const gfx::Rect& bounds);
};

}  

#endif  
