// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_STATUS_NETWORK_DROPDOWN_BUTTON_H_
#define CHROME_BROWSER_CHROMEOS_STATUS_NETWORK_DROPDOWN_BUTTON_H_
#pragma once

#include "chrome/browser/chromeos/cros/network_library.h"
#include "chrome/browser/chromeos/status/network_menu.h"
#include "chrome/browser/chromeos/views/dropdown_button.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/throb_animation.h"

namespace chromeos {

class NetworkDropdownButton : public DropDownButton,
                              public NetworkMenu,
                              public NetworkLibrary::NetworkManagerObserver {
 public:
  NetworkDropdownButton(bool browser_mode, gfx::NativeWindow parent_window);
  virtual ~NetworkDropdownButton();

  
  virtual void AnimationProgressed(const ui::Animation* animation);

  
  virtual void OnNetworkManagerChanged(NetworkLibrary* obj);

  
  void Refresh();

 protected:
  
  virtual bool IsBrowserMode() const { return browser_mode_; }
  virtual gfx::NativeWindow GetNativeWindow() const { return parent_window_; }
  virtual void OpenButtonOptions() {}
  virtual bool ShouldOpenButtonOptions() const { return false; }

 private:
  bool browser_mode_;

  
  ui::ThrobAnimation animation_connecting_;

  
  static const int kThrobDuration;

  gfx::NativeWindow parent_window_;

  DISALLOW_COPY_AND_ASSIGN(NetworkDropdownButton);
};

}  

#endif  
