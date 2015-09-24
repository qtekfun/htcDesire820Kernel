// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_RENDER_TEXT_WIN_H_
#define UI_GFX_RENDER_TEXT_WIN_H_

#include <usp10.h>

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "ui/gfx/render_text.h"

namespace gfx {

namespace internal {

struct TextRun {
  TextRun();
  ~TextRun();

  Range range;
  Font font;
  
  
  
  int font_style;

  bool strike;
  bool diagonal_strike;
  bool underline;

  int width;
  
  int preceding_run_widths;

  SCRIPT_ANALYSIS script_analysis;

  scoped_ptr<WORD[]> glyphs;
  scoped_ptr<WORD[]> logical_clusters;
  scoped_ptr<SCRIPT_VISATTR[]> visible_attributes;
  int glyph_count;

  scoped_ptr<int[]> advance_widths;
  scoped_ptr<GOFFSET[]> offsets;
  ABC abc_widths;
  SCRIPT_CACHE script_cache;

 private:
  DISALLOW_COPY_AND_ASSIGN(TextRun);
};

}  

class RenderTextWin : public RenderText {
 public:
  RenderTextWin();
  virtual ~RenderTextWin();

  
  virtual Size GetStringSize() OVERRIDE;
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
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, Win_BreakRunsByUnicodeBlocks);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, Win_LogicalClusters);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, Multiline_MinWidth);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, Multiline_NormalWidth);

  void ItemizeLogicalText();
  void LayoutVisualText();
  void LayoutTextRun(internal::TextRun* run);

  
  
  HRESULT ShapeTextRunWithFont(internal::TextRun* run, const Font& font);

  
  int CountCharsWithMissingGlyphs(internal::TextRun* run) const;

  
  
  size_t GetRunContainingCaret(const SelectionModel& caret) const;
  size_t GetRunContainingXCoord(int x) const;

  
  
  
  SelectionModel FirstSelectionModelInsideRun(const internal::TextRun* run);
  SelectionModel LastSelectionModelInsideRun(const internal::TextRun* run);

  
  static HDC cached_hdc_;

  
  
  static std::map<std::string, Font> successful_substitute_fonts_;

  SCRIPT_CONTROL script_control_;
  SCRIPT_STATE script_state_;

  ScopedVector<internal::TextRun> runs_;

  
  int string_width_;

  
  Size multiline_string_size_;

  scoped_ptr<int[]> visual_to_logical_;
  scoped_ptr<int[]> logical_to_visual_;

  bool needs_layout_;

  DISALLOW_COPY_AND_ASSIGN(RenderTextWin);
};

}  

#endif  
