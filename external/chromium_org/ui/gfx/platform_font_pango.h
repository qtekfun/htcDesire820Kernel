// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_PLATFORM_FONT_PANGO_H_
#define UI_GFX_PLATFORM_FONT_PANGO_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "skia/ext/refptr.h"
#include "third_party/skia/include/core/SkRefCnt.h"
#include "ui/gfx/platform_font.h"

class SkTypeface;
class SkPaint;

namespace gfx {

class GFX_EXPORT PlatformFontPango : public PlatformFont {
 public:
  PlatformFontPango();
  explicit PlatformFontPango(NativeFont native_font);
  PlatformFontPango(const std::string& font_name, int font_size);

  
  
  static PangoFontDescription* PangoFontFromGfxFont(const gfx::Font& gfx_font);

  
  
  
  static void ReloadDefaultFont();

#if defined(OS_CHROMEOS)
  
  static void SetDefaultFontDescription(const std::string& font_description);
#endif

  
  
  
  double underline_position() const;
  
  double underline_thickness() const;

  
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
  
  
  PlatformFontPango(const skia::RefPtr<SkTypeface>& typeface,
                    const std::string& name,
                    int size,
                    int style);
  virtual ~PlatformFontPango();

  
  
  static std::string GetDefaultFont();

  
  void InitWithNameAndSize(const std::string& font_name, int font_size);
  void InitWithTypefaceNameSizeAndStyle(
      const skia::RefPtr<SkTypeface>& typeface,
      const std::string& font_family,
      int font_size,
      int style);
  void InitFromPlatformFont(const PlatformFontPango* other);

  
  void InitPangoMetrics();

  
  void PaintSetup(SkPaint* paint) const;

  
  void CopyFont(const Font& other);

  
  double GetAverageWidth() const;

  skia::RefPtr<SkTypeface> typeface_;

  
  
  std::string font_family_;
  int font_size_pixels_;
  int style_;

  
  int height_pixels_;
  int ascent_pixels_;

  
  
  bool pango_metrics_inited_;
  double average_width_pixels_;
  double underline_position_pixels_;
  double underline_thickness_pixels_;

  
  static Font* default_font_;

#if defined(OS_CHROMEOS)
  static std::string* default_font_description_;
#endif

  DISALLOW_COPY_AND_ASSIGN(PlatformFontPango);
};

}  

#endif  
