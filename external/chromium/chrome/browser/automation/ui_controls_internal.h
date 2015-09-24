// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOMATION_UI_CONTROLS_INTERNAL_H_
#define CHROME_BROWSER_AUTOMATION_UI_CONTROLS_INTERNAL_H_

#include "base/task.h"
#include "chrome/browser/automation/ui_controls.h"

namespace ui_controls {

class ClickTask : public Task {
 public:
  
  
  ClickTask(MouseButton button, int state, Task* followup);
  virtual ~ClickTask();
  virtual void Run();

 private:
  MouseButton button_;
  int state_;
  Task* followup_;
};

}  

#endif  
