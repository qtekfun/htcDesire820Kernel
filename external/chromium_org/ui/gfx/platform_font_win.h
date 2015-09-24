// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_PLATFORM_FONT_WIN_H_
#define UI_GFX_PLATFORM_FONT_WIN_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/platform_font.h"

namespace gfx {

class GFX_EXPORT PlatformFontWin : public PlatformFont {
 public:
  PlatformFontWin();
  explicit PlatformFontWin(NativeFont native_font);
  PlatformFontWin(const std::string& font_name, int font_size);

  
  
  int horizontal_dlus_to_pixels(int dlus) const {
    return dlus * font_ref_->GetDluBaseX() / 4;
  }
  int vertical_dlus_to_pixels(int dlus)  const {
    return dlus * font_ref_->height() / 8;
  }

  
  
  typedef int (*GetMinimumFontSizeCallback)();
  static GetMinimumFontSizeCallback get_minimum_font_size_callback;

  
  
  
  
  typedef void (*AdjustFontCallback)(LOGFONT* lf);
  static AdjustFontCallback adjust_font_callback;

  
  
  
  std::string GetLocalizedFontName() const;

  
  
  
  
  
  Font DeriveFontWithHeight(int height, int style);

  
  virtual Font DeriveFont(int size_delta, int style) const OVERRIDE;
  virtual int GetHeight() const OVERRIDE;
  virtual int GetBaseline() const OVERRIDE;
  virtual int GetCapHeight() const OVERRIDE;
  virtual int GetAverageCharacterWidth() const OVERRIDE;
  virtual int GetStringWidth(const base::string16& text) const OVERRIDE;
  virtual int GetExpectedTextWidth(int length) const OVERRIDE;
  virtual int GetStyle() const OVERRIDE;
  virtual std::string GetFontName() const OVERRIDE;
  virtual std::string GetActualFontNameForTesting() const OVERRIDE;
  virtual int GetFontSize() const OVERRIDE;
  virtual NativeFont GetNativeFont() const OVERRIDE;

 private:
  virtual ~PlatformFontWin() {}

  
  
  
  
  
  
  
  
  class HFontRef : public base::RefCounted<HFontRef> {
   public:
    
    
    HFontRef(HFONT hfont,
             int font_size,
             int height,
             int baseline,
             int cap_height,
             int ave_char_width,
             int style);

    
    HFONT hfont() const { return hfont_; }
    int height() const { return height_; }
    int baseline() const { return baseline_; }
    int cap_height() const { return cap_height_; }
    int ave_char_width() const { return ave_char_width_; }
    int style() const { return style_; }
    const std::string& font_name() const { return font_name_; }
    int font_size() const { return font_size_; }
    int requested_font_size() const { return requested_font_size_; }

    
    int GetDluBaseX();

   private:
    friend class base::RefCounted<HFontRef>;

    ~HFontRef();

    const HFONT hfont_;
    const int font_size_;
    const int height_;
    const int baseline_;
    const int cap_height_;
    const int ave_char_width_;
    const int style_;
    
    
    int dlu_base_x_;
    std::string font_name_;

    
    
    
    
    int requested_font_size_;

    DISALLOW_COPY_AND_ASSIGN(HFontRef);
  };

  
  void InitWithCopyOfHFONT(HFONT hfont);

  
  void InitWithFontNameAndSize(const std::string& font_name,
                               int font_size);

  
  
  static HFontRef* GetBaseFontRef();

  
  static HFontRef* CreateHFontRef(HFONT font);

  
  
  explicit PlatformFontWin(HFontRef* hfont_ref);

    
  static HFontRef* base_font_ref_;

  
  scoped_refptr<HFontRef> font_ref_;

  DISALLOW_COPY_AND_ASSIGN(PlatformFontWin);
};

}  

#endif  
