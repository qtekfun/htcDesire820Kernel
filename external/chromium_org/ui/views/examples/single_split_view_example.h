// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_SINGLE_SPLIT_VIEW_EXAMPLE_H_
#define UI_VIEWS_EXAMPLES_SINGLE_SPLIT_VIEW_EXAMPLE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/controls/single_split_view_listener.h"
#include "ui/views/examples/example_base.h"

namespace views {
namespace examples {

class SingleSplitViewExample : public ExampleBase,
                               public SingleSplitViewListener {
 public:
  SingleSplitViewExample();
  virtual ~SingleSplitViewExample();

  
  virtual void CreateExampleView(View* container) OVERRIDE;

 private:
  virtual bool SplitHandleMoved(SingleSplitView* sender) OVERRIDE;

  SingleSplitView* single_split_view_;

  DISALLOW_COPY_AND_ASSIGN(SingleSplitViewExample);
};

}  
}  

#endif  