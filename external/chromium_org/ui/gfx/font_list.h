// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_FONT_LIST_H_
#define UI_GFX_FONT_LIST_H_

#include <string>
#include <vector>

#include "ui/gfx/font.h"
#include "ui/gfx/gfx_export.h"

namespace gfx {

class GFX_EXPORT FontList {
 public:
  
  FontList();

  
  
  explicit FontList(const std::string& font_description_string);

  
  FontList(const std::vector<std::string>& font_names,
           int font_style,
           int font_size);

  
  
  explicit FontList(const std::vector<Font>& fonts);

  
  explicit FontList(const Font& font);

  ~FontList();

  
  
  
  
  
  
  
  
  static void SetDefaultFontDescription(const std::string& font_description);

  
  FontList DeriveFontList(int font_style) const;

  
  
  FontList DeriveFontListWithSize(int size) const;

  
  
  FontList DeriveFontListWithSizeDelta(int size_delta) const;

  
  
  
  
  FontList DeriveFontListWithSizeDeltaAndStyle(int size_delta,
                                               int font_style) const;

  
  
  int GetHeight() const;

  
  
  int GetBaseline() const;

  
  
  int GetCapHeight() const;

  
  int GetStringWidth(const base::string16& text) const;

  
  
  
  int GetExpectedTextWidth(int length) const;

  
  int GetFontStyle() const;

  
  
  
  const std::string& GetFontDescriptionString() const;

  
  int GetFontSize() const;

  
  const std::vector<Font>& GetFonts() const;

  
  const Font& GetPrimaryFont() const;

 private:
  
  
  void CacheCommonFontHeightAndBaseline() const;

  
  void CacheFontStyleAndSize() const;

  
  
  
  mutable std::vector<Font> fonts_;

  
  
  
  
  
  
  mutable std::string font_description_string_;

  
  mutable int common_height_;
  mutable int common_baseline_;

  
  mutable int font_style_;
  mutable int font_size_;
};

}  

#endif  
