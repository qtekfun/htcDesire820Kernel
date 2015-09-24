// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_RESIZING_MODE_SELECTOR_H_
#define CONTENT_RENDERER_RESIZING_MODE_SELECTOR_H_

#include "base/basictypes.h"

struct ViewMsg_Resize_Params;

namespace content {

class RenderWidget;

// written with the expectation of a synchronous resize, and we're going to
class ResizingModeSelector {
 public:
  ResizingModeSelector();
  bool NeverUsesSynchronousResize() const;
  bool ShouldAbortOnResize(RenderWidget* widget,
                           const ViewMsg_Resize_Params& params);

  void set_is_synchronous_mode(bool mode);
  bool is_synchronous_mode() const;

 private:
  bool is_synchronous_mode_;

  DISALLOW_COPY_AND_ASSIGN(ResizingModeSelector);
};

}  

#endif  
