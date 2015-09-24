// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_RENDER_TEXT_H_
#define UI_GFX_RENDER_TEXT_H_

#include <algorithm>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/i18n/rtl.h"
#include "base/strings/string16.h"
#include "skia/ext/refptr.h"
#include "third_party/skia/include/core/SkColor.h"
#include "third_party/skia/include/core/SkPaint.h"
#include "third_party/skia/include/core/SkRect.h"
#include "ui/gfx/break_list.h"
#include "ui/gfx/font_list.h"
#include "ui/gfx/point.h"
#include "ui/gfx/range/range.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/selection_model.h"
#include "ui/gfx/shadow_value.h"
#include "ui/gfx/size_f.h"
#include "ui/gfx/text_constants.h"
#include "ui/gfx/vector2d.h"

class SkCanvas;
class SkDrawLooper;
struct SkPoint;
class SkShader;
class SkTypeface;

namespace gfx {

class Canvas;
class Font;
class RenderTextTest;

namespace internal {

class SkiaTextRenderer {
 public:
  explicit SkiaTextRenderer(Canvas* canvas);
  ~SkiaTextRenderer();

  void SetDrawLooper(SkDrawLooper* draw_looper);
  void SetFontSmoothingSettings(bool enable_smoothing, bool enable_lcd_text);
  void SetTypeface(SkTypeface* typeface);
  void SetTextSize(SkScalar size);
  void SetFontFamilyWithStyle(const std::string& family, int font_style);
  void SetForegroundColor(SkColor foreground);
  void SetShader(SkShader* shader, const Rect& bounds);
  
  
  
  void SetUnderlineMetrics(SkScalar thickness, SkScalar position);
  void DrawSelection(const std::vector<Rect>& selection, SkColor color);
  void DrawPosText(const SkPoint* pos,
                   const uint16* glyphs,
                   size_t glyph_count);
  
  
  
  void DrawDecorations(int x, int y, int width, bool underline, bool strike,
                       bool diagonal_strike);
  void DrawUnderline(int x, int y, int width);
  void DrawStrike(int x, int y, int width) const;
  void DrawDiagonalStrike(int x, int y, int width) const;

 private:
  SkCanvas* canvas_skia_;
  bool started_drawing_;
  SkPaint paint_;
  SkRect bounds_;
  skia::RefPtr<SkShader> deferred_fade_shader_;
  SkScalar underline_thickness_;
  SkScalar underline_position_;

  DISALLOW_COPY_AND_ASSIGN(SkiaTextRenderer);
};

class StyleIterator {
 public:
  StyleIterator(const BreakList<SkColor>& colors,
                const std::vector<BreakList<bool> >& styles);
  ~StyleIterator();

  
  SkColor color() const { return color_->second; }
  bool style(TextStyle s) const { return style_[s]->second; }

  
  Range GetRange() const;

  
  void UpdatePosition(size_t position);

 private:
  BreakList<SkColor> colors_;
  std::vector<BreakList<bool> > styles_;

  BreakList<SkColor>::const_iterator color_;
  std::vector<BreakList<bool>::const_iterator> style_;

  DISALLOW_COPY_AND_ASSIGN(StyleIterator);
};

struct LineSegment {
  LineSegment();
  ~LineSegment();

  
  Range x_range;

  
  Range char_range;

  
  size_t run;
};

struct Line {
  Line();
  ~Line();

  
  std::vector<LineSegment> segments;

  
  
  Size size;

  
  int preceding_heights;

  
  int baseline;
};

}  

class GFX_EXPORT RenderText {
 public:
  virtual ~RenderText();

  
  static RenderText* CreateInstance();

  const base::string16& text() const { return text_; }
  void SetText(const base::string16& text);

  HorizontalAlignment horizontal_alignment() const {
    return horizontal_alignment_;
  }
  void SetHorizontalAlignment(HorizontalAlignment alignment);

  const FontList& font_list() const { return font_list_; }
  void SetFontList(const FontList& font_list);
  void SetFont(const Font& font);

  
  void SetFontSize(int size);

  
  const Font& GetPrimaryFont() const;

  bool cursor_enabled() const { return cursor_enabled_; }
  void SetCursorEnabled(bool cursor_enabled);

  bool cursor_visible() const { return cursor_visible_; }
  void set_cursor_visible(bool visible) { cursor_visible_ = visible; }

  bool insert_mode() const { return insert_mode_; }
  void ToggleInsertMode();

  SkColor cursor_color() const { return cursor_color_; }
  void set_cursor_color(SkColor color) { cursor_color_ = color; }

  SkColor selection_color() const { return selection_color_; }
  void set_selection_color(SkColor color) { selection_color_ = color; }

  SkColor selection_background_focused_color() const {
    return selection_background_focused_color_;
  }
  void set_selection_background_focused_color(SkColor color) {
    selection_background_focused_color_ = color;
  }

  bool focused() const { return focused_; }
  void set_focused(bool focused) { focused_ = focused; }

  bool clip_to_display_rect() const { return clip_to_display_rect_; }
  void set_clip_to_display_rect(bool clip) { clip_to_display_rect_ = clip; }

  
  bool obscured() const { return obscured_; }
  void SetObscured(bool obscured);

  
  
  
  
  
  void SetObscuredRevealIndex(int index);

  
  
  bool multiline() const { return multiline_; }
  void SetMultiline(bool multiline);

  
  
  
  
  void set_truncate_length(size_t length) { truncate_length_ = length; }

  const Rect& display_rect() const { return display_rect_; }
  void SetDisplayRect(const Rect& r);

  void set_fade_head(bool fade_head) { fade_head_ = fade_head; }
  bool fade_head() const { return fade_head_; }
  void set_fade_tail(bool fade_tail) { fade_tail_ = fade_tail; }
  bool fade_tail() const { return fade_tail_; }

  bool background_is_transparent() const { return background_is_transparent_; }
  void set_background_is_transparent(bool transparent) {
    background_is_transparent_ = transparent;
  }

  const SelectionModel& selection_model() const { return selection_model_; }

  const Range& selection() const { return selection_model_.selection(); }

  size_t cursor_position() const { return selection_model_.caret_pos(); }
  void SetCursorPosition(size_t position);

  
  
  
  void MoveCursor(BreakType break_type,
                  VisualCursorDirection direction,
                  bool select);

  
  
  
  
  
  bool MoveCursorTo(const SelectionModel& selection_model);

  
  
  
  bool MoveCursorTo(const Point& point, bool select);

  
  
  
  
  
  bool SelectRange(const Range& range);

  
  bool IsPointInSelection(const Point& point);

  
  void ClearSelection();

  
  
  
  void SelectAll(bool reversed);

  
  
  
  void SelectWord();

  const Range& GetCompositionRange() const;
  void SetCompositionRange(const Range& composition_range);

  
  
  void SetColor(SkColor value);
  void ApplyColor(SkColor value, const Range& range);

  
  
  
  void SetStyle(TextStyle style, bool value);
  void ApplyStyle(TextStyle style, bool value, const Range& range);

  
  
  bool GetStyle(TextStyle style) const;

  
  void SetDirectionalityMode(DirectionalityMode mode);
  DirectionalityMode directionality_mode() const {
      return directionality_mode_;
  }
  base::i18n::TextDirection GetTextDirection();

  
  
  
  VisualCursorDirection GetVisualDirectionOfLogicalEnd();

  
  
  
  
  virtual Size GetStringSize() = 0;

  
  
  
  
  
  virtual SizeF GetStringSizeF();

  
  
  int GetContentWidth();

  
  
  
  
  int GetBaseline();

  void Draw(Canvas* canvas);

  
  void DrawCursor(Canvas* canvas, const SelectionModel& position);

  
  
  void DrawSelectedTextForDrag(Canvas* canvas);

  
  virtual SelectionModel FindCursorPosition(const Point& point) = 0;

  
  
  virtual bool IsCursorablePosition(size_t position) = 0;

  
  
  
  
  
  
  
  
  Rect GetCursorBounds(const SelectionModel& caret, bool insert_mode);

  
  
  
  const Rect& GetUpdatedCursorBounds();

  
  
  
  
  
  
  size_t IndexOfAdjacentGrapheme(size_t index,
                                 LogicalCursorDirection direction);

  
  
  SelectionModel GetSelectionModelForSelectionStart();

  
  void SetTextShadows(const ShadowValues& shadows);

  typedef std::pair<Font, Range> FontSpan;
  
  
  
  
  virtual std::vector<FontSpan> GetFontSpansForTesting() = 0;

 protected:
  RenderText();

  const BreakList<SkColor>& colors() const { return colors_; }
  const std::vector<BreakList<bool> >& styles() const { return styles_; }

  const std::vector<internal::Line>& lines() const { return lines_; }
  void set_lines(std::vector<internal::Line>* lines) { lines_.swap(*lines); }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int GetLayoutTextBaseline() = 0;

  const Vector2d& GetUpdatedDisplayOffset();

  void set_cached_bounds_and_offset_valid(bool valid) {
    cached_bounds_and_offset_valid_ = valid;
  }

  
  
  SelectionModel GetAdjacentSelectionModel(const SelectionModel& current,
                                           BreakType break_type,
                                           VisualCursorDirection direction);

  
  
  virtual SelectionModel AdjacentCharSelectionModel(
      const SelectionModel& selection,
      VisualCursorDirection direction) = 0;

  
  
  virtual SelectionModel AdjacentWordSelectionModel(
      const SelectionModel& selection,
      VisualCursorDirection direction) = 0;

  
  
  SelectionModel EdgeSelectionModel(VisualCursorDirection direction);

  
  virtual void SetSelectionModel(const SelectionModel& model);

  
  
  
  
  virtual Range GetGlyphBounds(size_t index) = 0;

  
  
  
  
  
  
  virtual std::vector<Rect> GetSubstringBounds(const Range& range) = 0;

  
  
  
  virtual size_t TextIndexToLayoutIndex(size_t index) const = 0;
  virtual size_t LayoutIndexToTextIndex(size_t index) const = 0;

  
  virtual void ResetLayout() = 0;

  
  virtual void EnsureLayout() = 0;

  
  virtual void DrawVisualText(Canvas* canvas) = 0;

  
  const base::string16& GetLayoutText() const;

  
  const BreakList<size_t>& GetLineBreaks();

  
  void ApplyCompositionAndSelectionStyles();
  void UndoCompositionAndSelectionStyles();

  
  
  Vector2d GetLineOffset(size_t line_number);

  
  
  Point ToTextPoint(const Point& point);
  Point ToViewPoint(const Point& point);

  
  
  std::vector<Rect> TextBoundsToViewBounds(const Range& x);

  
  
  Vector2d GetAlignmentOffset(size_t line_number);

  
  void ApplyFadeEffects(internal::SkiaTextRenderer* renderer);

  
  void ApplyTextShadows(internal::SkiaTextRenderer* renderer);

  
  
  static bool RangeContainsCaret(const Range& range,
                                 size_t caret_pos,
                                 LogicalCursorDirection caret_affinity);

 private:
  friend class RenderTextTest;
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, DefaultStyle);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, SetColorAndStyle);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, ApplyColorAndStyle);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, ObscuredText);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, RevealObscuredText);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, TruncatedText);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, TruncatedObscuredText);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, GraphemePositions);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, EdgeSelectionModels);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, GetTextOffset);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, GetTextOffsetHorizontalDefaultInRTL);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, Multiline_MinWidth);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, Multiline_NormalWidth);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, Multiline_SufficientWidth);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, Multiline_Newline);

  
  
  
  
  
  void MoveCursorTo(size_t position, bool select);

  
  void UpdateLayoutText();

  
  
  void UpdateCachedBoundsAndOffset();

  
  void DrawSelection(Canvas* canvas);

  
  base::string16 text_;

  
  
  HorizontalAlignment horizontal_alignment_;

  
  DirectionalityMode directionality_mode_;

  
  
  base::i18n::TextDirection text_direction_;

  
  FontList font_list_;

  
  SelectionModel selection_model_;

  
  Rect cursor_bounds_;

  
  
  bool cursor_enabled_;

  
  bool cursor_visible_;
  bool insert_mode_;

  
  SkColor cursor_color_;

  
  SkColor selection_color_;

  
  SkColor selection_background_focused_color_;

  
  bool focused_;

  
  Range composition_range_;

  
  
  
  BreakList<SkColor> colors_;
  std::vector<BreakList<bool> > styles_;

  
  BreakList<SkColor> saved_colors_;
  BreakList<bool> saved_underlines_;
  bool composition_and_selection_styles_applied_;

  
  bool obscured_;
  
  int obscured_reveal_index_;

  
  size_t truncate_length_;

  
  base::string16 layout_text_;

  
  
  bool multiline_;

  
  bool fade_head_;
  bool fade_tail_;

  
  bool background_is_transparent_;

  
  Rect display_rect_;

  
  
  
  
  bool clip_to_display_rect_;

  
  
  Vector2d display_offset_;

  
  
  
  int baseline_;

  
  
  bool cached_bounds_and_offset_valid_;

  
  ShadowValues text_shadows_;

  
  BreakList<size_t> line_breaks_;

  
  
  std::vector<internal::Line> lines_;

  DISALLOW_COPY_AND_ASSIGN(RenderText);
};

}  

#endif  
