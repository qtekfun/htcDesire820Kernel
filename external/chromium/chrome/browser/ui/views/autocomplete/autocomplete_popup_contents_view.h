// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_AUTOCOMPLETE_AUTOCOMPLETE_POPUP_CONTENTS_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_AUTOCOMPLETE_AUTOCOMPLETE_POPUP_CONTENTS_VIEW_H_
#pragma once

#include "chrome/browser/autocomplete/autocomplete.h"
#include "chrome/browser/autocomplete/autocomplete_popup_model.h"
#include "chrome/browser/autocomplete/autocomplete_popup_view.h"
#include "chrome/browser/ui/views/autocomplete/autocomplete_result_view_model.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/slide_animation.h"
#include "ui/gfx/font.h"
#include "views/view.h"
#include "webkit/glue/window_open_disposition.h"

#if defined(OS_WIN)
#include "chrome/browser/ui/views/autocomplete/autocomplete_popup_win.h"
#else
#include "chrome/browser/ui/views/autocomplete/autocomplete_popup_gtk.h"
#endif

class AutocompleteEditModel;
class AutocompleteEditViewWin;
struct AutocompleteMatch;
class AutocompleteResultView;
class BubbleBorder;
class Profile;

namespace gfx {
class CanvasSkia;
class Insets;
}

class AutocompletePopupContentsView : public views::View,
                                      public AutocompleteResultViewModel,
                                      public AutocompletePopupView,
                                      public ui::AnimationDelegate {
 public:
  AutocompletePopupContentsView(const gfx::Font& font,
                                AutocompleteEditView* edit_view,
                                AutocompleteEditModel* edit_model,
                                Profile* profile,
                                const views::View* location_bar);
  virtual ~AutocompletePopupContentsView();

  
  
  gfx::Rect GetPopupBounds() const;

  virtual void LayoutChildren();

  
  virtual bool IsOpen() const OVERRIDE;
  virtual void InvalidateLine(size_t line) OVERRIDE;
  virtual void UpdatePopupAppearance() OVERRIDE;
  virtual gfx::Rect GetTargetBounds() OVERRIDE;
  virtual void PaintUpdatesNow() OVERRIDE;
  virtual void OnDragCanceled() OVERRIDE;

  
  virtual bool IsSelectedIndex(size_t index) const OVERRIDE;
  virtual bool IsHoveredIndex(size_t index) const OVERRIDE;
  virtual const SkBitmap* GetIconIfExtensionMatch(size_t index) const OVERRIDE;

  
  virtual void AnimationProgressed(const ui::Animation* animation) OVERRIDE;

  
  virtual void Layout() OVERRIDE;
  virtual views::View* GetEventHandlerForPoint(
      const gfx::Point& point) OVERRIDE;
  virtual bool OnMousePressed(const views::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnMouseMoved(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseEntered(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const views::MouseEvent& event) OVERRIDE;

 protected:
  virtual void PaintResultViews(gfx::CanvasSkia* canvas);

  
  virtual int CalculatePopupHeight();
  virtual AutocompleteResultView* CreateResultView(
      AutocompleteResultViewModel* model,
      int model_index,
      const gfx::Font& font,
      const gfx::Font& bold_font);

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  
  
  
  
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;

  scoped_ptr<AutocompletePopupModel> model_;

  
  views::View* opt_in_view_;

 private:
#if defined(OS_WIN)
  typedef AutocompletePopupWin AutocompletePopupClass;
#else
  typedef AutocompletePopupGtk AutocompletePopupClass;
#endif
  class InstantOptInView;

  
  bool HasMatchAt(size_t index) const;

  
  const AutocompleteMatch& GetMatchAtIndex(size_t index) const;

  
  
  void MakeContentsPath(gfx::Path* path, const gfx::Rect& bounding_rect);

  
  void UpdateBlurRegion();

  
  void MakeCanvasTransparent(gfx::Canvas* canvas);

  
  
  void OpenIndex(size_t index, WindowOpenDisposition disposition);

  
  
  
  size_t GetIndexForPoint(const gfx::Point& point);

  
  gfx::Rect CalculateTargetBounds(int h);

  
  
  void UserPressedOptIn(bool opt_in);

  
  
  
  
  base::WeakPtr<AutocompletePopupClass> popup_;

  
  AutocompleteEditView* edit_view_;

  
  const views::View* location_bar_;

  
  const BubbleBorder* bubble_border_;

  
  
  gfx::Font result_font_;

  
  gfx::Font result_bold_font_;

  
  
  
  
  
  
  bool ignore_mouse_drag_;

  
  
  ui::SlideAnimation size_animation_;
  gfx::Rect start_bounds_;
  gfx::Rect target_bounds_;

  DISALLOW_COPY_AND_ASSIGN(AutocompletePopupContentsView);
};

#endif  
