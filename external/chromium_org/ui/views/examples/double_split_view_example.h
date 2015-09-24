// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_DOUBLE_SPLIT_VIEW_EXAMPLE_H_
#define UI_VIEWS_EXAMPLES_DOUBLE_SPLIT_VIEW_EXAMPLE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/examples/example_base.h"

namespace views {
class SingleSplitView;

namespace examples {

class DoubleSplitViewExample : public ExampleBase {
 public:
  DoubleSplitViewExample();
  virtual ~DoubleSplitViewExample();

  
  virtual void CreateExampleView(View* container) OVERRIDE;

 private:
  
  SingleSplitView* outer_single_split_view_;
  SingleSplitView* inner_single_split_view_;

  DISALLOW_COPY_AND_ASSIGN(DoubleSplitViewExample);
};

}  
}  

#endif  
