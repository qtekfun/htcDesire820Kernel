// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_EXAMPLE_BASE_H_
#define UI_VIEWS_EXAMPLES_EXAMPLE_BASE_H_

#include <string>

#include "base/basictypes.h"

namespace views {
class View;

namespace examples {

class ExampleBase {
 public:
  virtual ~ExampleBase();

  
  virtual void CreateExampleView(View* parent) = 0;

  const std::string& example_title() const { return example_title_; }
  View* example_view() { return container_; }

 protected:
  explicit ExampleBase(const char* title);

  View* container() { return container_; }

  
  void PrintStatus(const char* format, ...);

  
  const char* BoolToOnOff(bool value) {
    return value ? "on" : "off";
  }

 private:
  
  std::string example_title_;

  
  View* container_;

  DISALLOW_COPY_AND_ASSIGN(ExampleBase);
};

}  
}  

#endif  
