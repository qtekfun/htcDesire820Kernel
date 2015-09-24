// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_STATUS_STATUS_AREA_HOST_H_
#define CHROME_BROWSER_CHROMEOS_STATUS_STATUS_AREA_HOST_H_
#pragma once

#include "ui/gfx/native_widget_types.h"

namespace views {
class View;
}  

class Profile;

namespace chromeos {

class StatusAreaHost {
 public:
  
  enum TextStyle {
    kWhitePlain,
    kWhiteHaloed,
    kGrayEmbossed
  };

  
  enum ScreenMode {
    kLoginMode,  
    kBrowserMode,  
    kScreenLockerMode,  
  };

  
  
  virtual Profile* GetProfile() const = 0;

  
  virtual gfx::NativeWindow GetNativeWindow() const = 0;

  
  
  virtual bool ShouldOpenButtonOptions(
      const views::View* button_view) const = 0;

  
  virtual void OpenButtonOptions(const views::View* button_view) = 0;

  
  virtual void ExecuteBrowserCommand(int id) const = 0;

  
  virtual TextStyle GetTextStyle() const = 0;

  
  virtual ScreenMode GetScreenMode() const = 0;

 protected:
  virtual ~StatusAreaHost() {}
};

}  

#endif  
