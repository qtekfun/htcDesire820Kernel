// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_FIRST_RUN_FIRST_RUN_ACTOR_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_FIRST_RUN_FIRST_RUN_ACTOR_H_

#include <string>

#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
}

namespace chromeos {

class FirstRunActor {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void OnActorInitialized() = 0;

    
    virtual void OnNextButtonClicked(const std::string& step_name) = 0;

    
    virtual void OnHelpButtonClicked() = 0;

    
    virtual void OnStepShown(const std::string& step_name) = 0;

    
    virtual void OnStepHidden(const std::string& step_name) = 0;

    
    virtual void OnActorFinalized() = 0;

    
    virtual void OnActorDestroyed() = 0;
  };

  class StepPosition {
   public:
    
    StepPosition();

    
    StepPosition& SetTop(int top);
    StepPosition& SetRight(int right);
    StepPosition& SetBottom(int bottom);
    StepPosition& SetLeft(int left);

    
    scoped_ptr<base::DictionaryValue> AsValue() const;

   private:
    int top_;
    int right_;
    int bottom_;
    int left_;
  };

  FirstRunActor();
  virtual ~FirstRunActor();

  
  
  virtual bool IsInitialized() = 0;

  
  virtual void SetBackgroundVisible(bool visible) = 0;

  
  virtual void AddRectangularHole(int x, int y, int width, int height) = 0;

  
  virtual void AddRoundHole(int x, int y, float radius) = 0;

  
  virtual void RemoveBackgroundHoles() = 0;

  
  virtual void ShowStepPositioned(const std::string& name,
                                  const StepPosition& position) = 0;

  
  virtual void ShowStepPointingTo(const std::string& name,
                                  int x,
                                  int y,
                                  int offset) = 0;

  
  virtual void HideCurrentStep() = 0;

  
  virtual void Finalize() = 0;

  
  virtual bool IsFinalizing() = 0;

  void set_delegate(Delegate* delegate) { delegate_ = delegate; }
  Delegate* delegate() const { return delegate_; }

 private:
  Delegate* delegate_;
};

}  

#endif  

