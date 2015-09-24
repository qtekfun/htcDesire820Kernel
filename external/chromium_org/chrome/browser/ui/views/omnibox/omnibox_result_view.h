// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_OMNIBOX_OMNIBOX_RESULT_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_OMNIBOX_OMNIBOX_RESULT_VIEW_H_

#include <vector>

#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/slide_animation.h"
#include "ui/gfx/font_list.h"
#include "ui/gfx/rect.h"
#include "ui/views/controls/image_view.h"
#include "ui/views/view.h"

class LocationBarView;
class OmniboxResultViewModel;

namespace gfx {
class Canvas;
class RenderText;
}

class OmniboxResultView : public views::View,
                          private gfx::AnimationDelegate {
 public:
  
  
  enum ResultViewState {
    NORMAL = 0,
    HOVERED,
    SELECTED,
    NUM_STATES
  };

  enum ColorKind {
    BACKGROUND = 0,
    TEXT,
    DIMMED_TEXT,
    URL,
    DIVIDER,
    NUM_KINDS
  };

  OmniboxResultView(OmniboxResultViewModel* model,
                    int model_index,
                    LocationBarView* location_bar_view,
                    const gfx::FontList& font_list);
  virtual ~OmniboxResultView();

  SkColor GetColor(ResultViewState state, ColorKind kind) const;

  
  
  
  void SetMatch(const AutocompleteMatch& match);

  void ShowKeyword(bool show_keyword);

  void Invalidate();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  ResultViewState GetState() const;

  
  
  virtual int GetTextHeight() const;

 protected:
  virtual void PaintMatch(gfx::Canvas* canvas,
                          const AutocompleteMatch& match,
                          int x);

  
  
  
  
  int DrawString(gfx::Canvas* canvas,
                 const base::string16& text,
                 const ACMatchClassifications& classifications,
                 bool force_dim,
                 int x,
                 int y);

  const gfx::Rect& text_bounds() const { return text_bounds_; }

  void set_edge_item_padding(int value) { edge_item_padding_ = value; }
  void set_item_padding(int value) { item_padding_ = value; }
  void set_minimum_text_vertical_padding(int value) {
    minimum_text_vertical_padding_ = value;
  }

 private:
  struct RunData;
  typedef std::vector<RunData> Runs;
  typedef std::vector<gfx::RenderText*> Classifications;

  
  static void CommonInitColors(const ui::NativeTheme* theme,
                               SkColor colors[][NUM_KINDS]);

  
  static bool SortRunsLogically(const RunData& lhs, const RunData& rhs);
  static bool SortRunsVisually(const RunData& lhs, const RunData& rhs);

  gfx::ImageSkia GetIcon() const;
  const gfx::ImageSkia* GetKeywordIcon() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void Elide(Runs* runs, int remaining_width) const;

  
  virtual void Layout() OVERRIDE;
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

  static int default_icon_size_;

  
  int edge_item_padding_;
  int item_padding_;
  int minimum_text_vertical_padding_;

  
  OmniboxResultViewModel* model_;
  size_t model_index_;

  LocationBarView* location_bar_view_;

  const gfx::FontList font_list_;
  int font_height_;

  
  int ellipsis_width_;

  
  class MirroringContext;
  scoped_ptr<MirroringContext> mirroring_context_;

  AutocompleteMatch match_;

  gfx::Rect text_bounds_;
  gfx::Rect icon_bounds_;

  gfx::Rect keyword_text_bounds_;
  scoped_ptr<views::ImageView> keyword_icon_;

  scoped_ptr<gfx::SlideAnimation> animation_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxResultView);
};

#endif  
