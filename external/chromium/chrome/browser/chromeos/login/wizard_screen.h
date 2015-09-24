// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_SCREEN_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_SCREEN_H_
#pragma once

#include "ui/gfx/canvas.h"

class WizardScreen;
namespace chromeos {
class ScreenObserver;
}  
namespace gfx {
class Size;
}  
namespace views {
class View;
}  

class WizardScreenDelegate {
 public:
  
  virtual views::View* GetWizardView() = 0;

  
  virtual chromeos::ScreenObserver* GetObserver(WizardScreen* screen) = 0;

  
  virtual void ShowCurrentScreen() = 0;

 protected:
  virtual ~WizardScreenDelegate() {}
};

class WizardScreen {
 public:
  
  virtual void Show() = 0;
  
  virtual void Hide() = 0;
  
  virtual gfx::Size GetScreenSize() const = 0;

 protected:
  explicit WizardScreen(WizardScreenDelegate* delegate): delegate_(delegate) {}
  virtual ~WizardScreen() {}

  WizardScreenDelegate* delegate() { return delegate_; }

  
  virtual void Refresh() = 0;

 private:
  WizardScreenDelegate* delegate_;
};

#endif  
