// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_V2_PUBLIC_VIEW_OBSERVER_H_
#define UI_V2_PUBLIC_VIEW_OBSERVER_H_

#include "ui/v2/public/v2_export.h"

namespace gfx {
class Rect;
}

namespace v2 {

class View;

class V2_EXPORT ViewObserver {
 public:
  
  
  enum DispositionChangePhase {
    DISPOSITION_CHANGING,
    DISPOSITION_CHANGED
  };

  
  struct TreeChangeParams {
    TreeChangeParams();
    View* target;
    View* old_parent;
    View* new_parent;
    View* receiver;
    DispositionChangePhase phase;
  };

  
  
  
  
  
  
  
  
  
  
  virtual void OnViewTreeChange(const TreeChangeParams& params) {}

  
  virtual void OnViewDestroy(View* view, DispositionChangePhase phase) {}
  virtual void OnViewBoundsChanged(View* view,
                                   const gfx::Rect& old_bounds,
                                   const gfx::Rect& new_bounds) {}
  virtual void OnViewVisibilityChange(View* view,
                                      DispositionChangePhase phase) {}

 protected:
  virtual ~ViewObserver() {}
};

}  

#endif  
