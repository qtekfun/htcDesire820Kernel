// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_AUTOCOMPLETE_AUTOCOMPLETE_RESULT_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_AUTOCOMPLETE_AUTOCOMPLETE_RESULT_VIEW_H_
#pragma once

#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/font.h"
#include "ui/gfx/rect.h"
#include "views/view.h"

class AutocompleteResultViewModel;
namespace gfx {
class Canvas;
}

class AutocompleteResultView : public views::View {
 public:
  enum ResultViewState {
    NORMAL = 0,
    SELECTED,
    HOVERED,
    NUM_STATES
  };

  enum ColorKind {
    BACKGROUND = 0,
    TEXT,
    DIMMED_TEXT,
    URL,
    NUM_KINDS
  };

  AutocompleteResultView(AutocompleteResultViewModel* model,
                         int model_index,
                         const gfx::Font& font,
                         const gfx::Font& bold_font);
  virtual ~AutocompleteResultView();

  static SkColor GetColor(ResultViewState state, ColorKind kind);

  
  
  
  void SetMatch(const AutocompleteMatch& match);

 protected:
  virtual void PaintMatch(gfx::Canvas* canvas,
                          const AutocompleteMatch& match,
                          int x);
  virtual int GetFontHeight() const;

  
  
  
  
  int DrawString(gfx::Canvas* canvas,
                 const string16& text,
                 const ACMatchClassifications& classifications,
                 bool force_dim,
                 int x,
                 int y);

  const gfx::Rect& text_bounds() const { return text_bounds_; }

 private:
  struct ClassificationData;
  typedef std::vector<ClassificationData> Classifications;

  struct RunData;
  typedef std::vector<RunData> Runs;

  
  static bool SortRunsLogically(const RunData& lhs, const RunData& rhs);
  static bool SortRunsVisually(const RunData& lhs, const RunData& rhs);

  ResultViewState GetState() const;
  const SkBitmap* GetIcon() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void Elide(Runs* runs, int remaining_width) const;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  static int default_icon_size_;

  
  AutocompleteResultViewModel* model_;
  size_t model_index_;

  const gfx::Font normal_font_;
  const gfx::Font bold_font_;

  
  int ellipsis_width_;

  
  class MirroringContext;
  scoped_ptr<MirroringContext> mirroring_context_;

  AutocompleteMatch match_;

  gfx::Rect text_bounds_;
  gfx::Rect icon_bounds_;

  DISALLOW_COPY_AND_ASSIGN(AutocompleteResultView);
};

#endif  
