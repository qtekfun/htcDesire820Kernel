// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MANAGED_MODE_MANAGED_USER_THEME_H_
#define CHROME_BROWSER_MANAGED_MODE_MANAGED_USER_THEME_H_

#include "chrome/browser/themes/custom_theme_supplier.h"

namespace gfx {
class Image;
}

class ManagedUserTheme : public CustomThemeSupplier {
 public:
  ManagedUserTheme();

  
  virtual bool GetColor(int id, SkColor* color) const OVERRIDE;
  virtual gfx::Image GetImageNamed(int id) OVERRIDE;
  virtual bool HasCustomImage(int id) const OVERRIDE;

 private:
  virtual ~ManagedUserTheme();

  DISALLOW_COPY_AND_ASSIGN(ManagedUserTheme);
};

#endif  
