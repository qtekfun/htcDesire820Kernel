// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_THEMES_CUSTOM_THEME_SUPPLIER_H_
#define CHROME_BROWSER_THEMES_CUSTOM_THEME_SUPPLIER_H_

#include "base/memory/ref_counted.h"
#include "content/public/browser/browser_thread.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/layout.h"

namespace base {
class RefCountedMemory;
}

namespace color_utils {
struct HSL;
}

namespace gfx {
class Image;
}

class CustomThemeSupplier : public base::RefCountedThreadSafe<
    CustomThemeSupplier, content::BrowserThread::DeleteOnFileThread> {
 public:
  enum ThemeType {
    EXTENSION,
    NATIVE_X11,
    MANAGED_USER_THEME,
  };

  explicit CustomThemeSupplier(ThemeType type);

  ThemeType get_theme_type() const {
    return theme_type_;
  }

  
  virtual void StartUsingTheme();

  
  virtual void StopUsingTheme();

  
  
  
  
  virtual bool GetTint(int id, color_utils::HSL* hsl) const;
  virtual bool GetColor(int id, SkColor* color) const;
  virtual bool GetDisplayProperty(int id, int* result) const;

  
  
  virtual gfx::Image GetImageNamed(int id);

  
  
  virtual base::RefCountedMemory* GetRawData(
      int id, ui::ScaleFactor scale_factor) const;

  
  virtual bool HasCustomImage(int id) const;

 protected:
  virtual ~CustomThemeSupplier();

 private:
  friend struct content::BrowserThread::DeleteOnThread<
      content::BrowserThread::FILE>;
  friend class base::DeleteHelper<CustomThemeSupplier>;

  ThemeType theme_type_;

  DISALLOW_COPY_AND_ASSIGN(CustomThemeSupplier);
};

#endif  
