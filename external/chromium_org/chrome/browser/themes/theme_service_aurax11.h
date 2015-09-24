// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_THEMES_THEME_SERVICE_AURAX11_H_
#define CHROME_BROWSER_THEMES_THEME_SERVICE_AURAX11_H_

#include "chrome/browser/themes/theme_service.h"

class ThemeServiceAuraX11 : public ThemeService {
 public:
  ThemeServiceAuraX11();
  virtual ~ThemeServiceAuraX11();

  
  virtual bool ShouldInitWithNativeTheme() const OVERRIDE;
  virtual void SetNativeTheme() OVERRIDE;
  virtual bool UsingDefaultTheme() const OVERRIDE;
  virtual bool UsingNativeTheme() const OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(ThemeServiceAuraX11);
};

#endif  
