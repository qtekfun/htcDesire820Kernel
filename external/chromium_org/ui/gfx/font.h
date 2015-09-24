// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_FONT_H_
#define UI_GFX_FONT_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/native_widget_types.h"

namespace gfx {

class PlatformFont;

class GFX_EXPORT Font {
 public:
  
  enum FontStyle {
    NORMAL = 0,
    BOLD = 1,
    ITALIC = 2,
    UNDERLINE = 4,
  };

  
  Font();

  
  Font(const Font& other);
  gfx::Font& operator=(const Font& other);

  
  explicit Font(NativeFont native_font);

  
  
  explicit Font(PlatformFont* platform_font);

  
  Font(const std::string& font_name, int font_size);

  ~Font();

  
  
  
  Font DeriveFont(int size_delta) const;

  
  
  
  
  
  Font DeriveFont(int size_delta, int style) const;

  
  
  
  
  
  int GetHeight() const;

  
  int GetBaseline() const;

  
  int GetCapHeight() const;

  
  int GetAverageCharacterWidth() const;

  
  
  int GetStringWidth(const base::string16& text) const;

  
  
  
  int GetExpectedTextWidth(int length) const;

  
  int GetStyle() const;

  
  std::string GetFontName() const;

  
  std::string GetActualFontNameForTesting() const;

  
  int GetFontSize() const;

  
  
  
  
  
  
  
  
  NativeFont GetNativeFont() const;

  
  
  PlatformFont* platform_font() const { return platform_font_.get(); }

 private:
  
  scoped_refptr<PlatformFont> platform_font_;
};

}  

#endif  
