// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_MENU_EXAMPLE_H_
#define UI_VIEWS_EXAMPLES_MENU_EXAMPLE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/examples/example_base.h"

namespace views {
namespace examples {

class MenuExample : public ExampleBase {
 public:
  MenuExample();
  virtual ~MenuExample();

  
  virtual void CreateExampleView(View* container) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(MenuExample);
};

}  
}  

#endif  
