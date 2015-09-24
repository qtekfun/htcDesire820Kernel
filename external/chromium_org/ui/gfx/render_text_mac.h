// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_RENDER_TEXT_MAC_H_
#define UI_GFX_RENDER_TEXT_MAC_H_

#include <ApplicationServices/ApplicationServices.h>

#include <string>
#include <vector>

#include "base/mac/scoped_cftyperef.h"
#include "ui/gfx/render_text.h"

namespace gfx {

class RenderTextMac : public RenderText {
 public:
  RenderTextMac();
  virtual ~RenderTextMac();

  
  virtual Size GetStringSize() OVERRIDE;
  virtual SizeF GetStringSizeF() OVERRIDE;
  virtual SelectionModel FindCursorPosition(const Point& point) OVERRIDE;
  virtual std::vector<FontSpan> GetFontSpansForTesting() OVERRIDE;

 protected:
  
  virtual int GetLayoutTextBaseline() OVERRIDE;
  virtual SelectionModel AdjacentCharSelectionModel(
      const SelectionModel& selection,
      VisualCursorDirection direction) OVERRIDE;
  virtual SelectionModel AdjacentWordSelectionModel(
      const SelectionModel& selection,
      VisualCursorDirection direction) OVERRIDE;
  virtual Range GetGlyphBounds(size_t index) OVERRIDE;
  virtual std::vector<Rect> GetSubstringBounds(const Range& range) OVERRIDE;
  virtual size_t TextIndexToLayoutIndex(size_t index) const OVERRIDE;
  virtual size_t LayoutIndexToTextIndex(size_t index) const OVERRIDE;
  virtual bool IsCursorablePosition(size_t position) OVERRIDE;
  virtual void ResetLayout() OVERRIDE;
  virtual void EnsureLayout() OVERRIDE;
  virtual void DrawVisualText(Canvas* canvas) OVERRIDE;

 private:
  struct TextRun {
    CTRunRef ct_run;
    SkPoint origin;
    std::vector<uint16> glyphs;
    std::vector<SkPoint> glyph_positions;
    SkScalar width;
    std::string font_name;
    int font_style;
    SkScalar text_size;
    SkColor foreground;
    bool underline;
    bool strike;
    bool diagonal_strike;

    TextRun();
    ~TextRun();
  };

  
  void ApplyStyles(CFMutableAttributedStringRef attr_string, CTFontRef ct_font);

  
  void ComputeRuns();

  
  base::ScopedCFTypeRef<CTLineRef> line_;

  
  
  base::ScopedCFTypeRef<CFMutableArrayRef> attributes_;

  
  SizeF string_size_;

  
  SkScalar common_baseline_;

  
  
  std::vector<TextRun> runs_;

  
  bool runs_valid_;

  DISALLOW_COPY_AND_ASSIGN(RenderTextMac);
};

}  

#endif  
