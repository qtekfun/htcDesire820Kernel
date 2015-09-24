// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_BASE_SCREEN_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_BASE_SCREEN_H_

#include <string>

#include "base/basictypes.h"
#include "chrome/browser/chromeos/login/screens/screen_context.h"

namespace base {
class DictionaryValue;
}

namespace chromeos {

class BaseScreen {
 public:
  BaseScreen();
  virtual ~BaseScreen();

  

  virtual void PrepareToShow() = 0;

  
  virtual void Show() = 0;

  
  virtual void Hide() = 0;

  
  virtual std::string GetName() const = 0;

  

  
  
  
  
  virtual void Initialize(ScreenContext* context);

  
  virtual void OnShow();
  
  
  virtual void OnHide();

  
  
  
  virtual void OnClose();

  
  
  virtual bool IsStatusAreaDisplayed();

  
  
  virtual bool IsPermanent();

  
  virtual std::string GetID() const;

 protected:
  
  
  void Finish(const std::string& outcome);

  
  
  virtual void OnButtonPressed(const std::string& button_id);

  
  
  
  virtual void OnContextChanged(const base::DictionaryValue* diff);

 private:
  friend class ScreenManager;
  void SetContext(ScreenContext* context);

  DISALLOW_COPY_AND_ASSIGN(BaseScreen);
};

}  

#endif  
