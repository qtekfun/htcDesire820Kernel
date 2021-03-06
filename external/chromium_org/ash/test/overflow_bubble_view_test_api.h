// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_TEST_OVERFLOW_BUBBLE_VIEW_TEST_API_
#define ASH_TEST_OVERFLOW_BUBBLE_VIEW_TEST_API_

#include "base/basictypes.h"

namespace gfx {
class Size;
}

namespace ash {

namespace internal {
class OverflowBubbleView;
}

namespace test {

class OverflowBubbleViewTestAPI {
 public:
  explicit OverflowBubbleViewTestAPI(internal::OverflowBubbleView* bubble_view);
  ~OverflowBubbleViewTestAPI();

  
  gfx::Size GetContentsSize();

  
  
  void ScrollByXOffset(int x_offset);

 private:
  internal::OverflowBubbleView* bubble_view_;

  DISALLOW_COPY_AND_ASSIGN(OverflowBubbleViewTestAPI);
};

}  
}  

#endif  
