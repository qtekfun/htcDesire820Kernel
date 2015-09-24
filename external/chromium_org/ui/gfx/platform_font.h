// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_PLATFORM_FONT_H_
#define UI_GFX_PLATFORM_FONT_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/native_widget_types.h"

namespace gfx {

class Font;

class GFX_EXPORT PlatformFont : public base::RefCounted<PlatformFont> {
 public:
  
  static PlatformFont* CreateDefault();
  static PlatformFont* CreateFromNativeFont(NativeFont native_font);
  
  
  static PlatformFont* CreateFromNameAndSize(const std::string& font_name,
                                             int font_size);

  
  
  
  
  virtual Font DeriveFont(int size_delta, int style) const = 0;

  
  
  
  
  
  virtual int GetHeight() const = 0;

  
  virtual int GetBaseline() const = 0;

  
  virtual int GetCapHeight() const = 0;

  
  virtual int GetAverageCharacterWidth() const = 0;

  
  
  virtual int GetStringWidth(const base::string16& text) const = 0;

  
  
  
  virtual int GetExpectedTextWidth(int length) const = 0;

  
  virtual int GetStyle() const = 0;

  
  virtual std::string GetFontName() const = 0;

  
  virtual std::string GetActualFontNameForTesting() const = 0;

  
  virtual int GetFontSize() const = 0;

  
  virtual NativeFont GetNativeFont() const = 0;

 protected:
  virtual ~PlatformFont() {}

 private:
  friend class base::RefCounted<PlatformFont>;
};

}  

#endif  
