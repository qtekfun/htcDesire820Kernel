// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_SCREENLOCK_ICON_PROVIDER_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_SCREENLOCK_ICON_PROVIDER_H_

#include <map>

#include "base/memory/weak_ptr.h"
#include "ui/gfx/image/image.h"

namespace chromeos {

class ScreenlockIconProvider
    : public base::SupportsWeakPtr<ScreenlockIconProvider> {
 public:
  ScreenlockIconProvider();
  ~ScreenlockIconProvider();

  
  void AddIcon(const std::string& username, const gfx::Image& icon);

  
  
  gfx::Image GetIcon(const std::string& username);

  
  void Clear();

 private:
  
  std::map<std::string, gfx::Image> user_icon_map_;

  DISALLOW_COPY_AND_ASSIGN(ScreenlockIconProvider);
};

}  

#endif  
