// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FIRST_RUN_STEP_H_
#define CHROME_BROWSER_CHROMEOS_FIRST_RUN_STEP_H_

#include <string>

#include "base/basictypes.h"
#include "base/time/time.h"

namespace ash {
class FirstRunHelper;
}

namespace gfx {
class Size;
}

namespace chromeos {

class FirstRunActor;

namespace first_run {

class Step {
 public:
  Step(const std::string& name,
       ash::FirstRunHelper* shell_helper,
       FirstRunActor* actor);
  virtual ~Step();

  
  void Show();

  
  void OnBeforeHide();

  
  void OnAfterHide();

  const std::string& name() const { return name_; }

 protected:
  ash::FirstRunHelper* shell_helper() const { return shell_helper_; }
  FirstRunActor* actor() const { return actor_; }
  gfx::Size GetOverlaySize() const;

  
  virtual void DoShow() = 0;

  
  
  virtual void DoOnBeforeHide() {}

  
  
  virtual void DoOnAfterHide() {}

 private:
  
  void RecordCompletion();

  std::string name_;
  ash::FirstRunHelper* shell_helper_;
  FirstRunActor* actor_;
  base::Time show_time_;

  DISALLOW_COPY_AND_ASSIGN(Step);
};

}  
}  

#endif  

