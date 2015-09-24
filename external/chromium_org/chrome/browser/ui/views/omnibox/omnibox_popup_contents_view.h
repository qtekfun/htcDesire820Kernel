// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_OMNIBOX_OMNIBOX_POPUP_CONTENTS_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_OMNIBOX_OMNIBOX_POPUP_CONTENTS_VIEW_H_

#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/omnibox/omnibox_popup_model.h"
#include "chrome/browser/ui/omnibox/omnibox_popup_view.h"
#include "chrome/browser/ui/views/omnibox/omnibox_result_view_model.h"
#include "ui/base/window_open_disposition.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/slide_animation.h"
#include "ui/gfx/font_list.h"
#include "ui/views/view.h"

struct AutocompleteMatch;
class LocationBarView;
class OmniboxEditModel;
class OmniboxResultView;
class OmniboxView;
class Profile;

class OmniboxPopupContentsView : public views::View,
                                 public OmniboxResultViewModel,
                                 public OmniboxPopupView,
                                 public gfx::AnimationDelegate {
 public:
  
  static OmniboxPopupView* Create(const gfx::FontList& font_list,
                                  OmniboxView* omnibox_view,
                                  OmniboxEditModel* edit_model,
                                  LocationBarView* location_bar_view);

  
  
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
  virtual gfx::Image GetIconIfExtensionMatch(size_t index) const OVERRIDE;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

  
  virtual void Layout() OVERRIDE;
  virtual views::View* GetEventHandlerForRect(
      const gfx::Rect& rect) OVERRIDE;
  virtual views::View* GetTooltipHandlerForPoint(
      const gfx::Point& point) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnMouseMoved(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

 protected:
  OmniboxPopupContentsView(const gfx::FontList& font_list,
                           OmniboxView* omnibox_view,
                           OmniboxEditModel* edit_model,
                           LocationBarView* location_bar_view);
  virtual ~OmniboxPopupContentsView();

  LocationBarView* location_bar_view() { return location_bar_view_; }

  virtual void PaintResultViews(gfx::Canvas* canvas);

  
  virtual int CalculatePopupHeight();
  virtual OmniboxResultView* CreateResultView(OmniboxResultViewModel* model,
                                              int model_index,
                                              const gfx::FontList& font_list);

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  
  
  
  
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;

  scoped_ptr<OmniboxPopupModel> model_;

 private:
  class AutocompletePopupWidget;

  
  void Init();

  
  bool HasMatchAt(size_t index) const;

  
  const AutocompleteMatch& GetMatchAtIndex(size_t index) const;

  
  
  void MakeContentsPath(gfx::Path* path, const gfx::Rect& bounding_rect);

  
  
  
  size_t GetIndexForPoint(const gfx::Point& point);

  
  
  void UpdateLineEvent(const ui::LocatedEvent& event,
                       bool should_set_selected_line);

  
  
  void OpenSelectedLine(const ui::LocatedEvent& event,
                        WindowOpenDisposition disposition);

  OmniboxResultView* result_view_at(size_t i);

  
  
  
  
  base::WeakPtr<AutocompletePopupWidget> popup_;

  
  OmniboxView* omnibox_view_;

  LocationBarView* location_bar_view_;

  
  gfx::FontList font_list_;

  
  
  
  
  
  
  bool ignore_mouse_drag_;

  
  
  gfx::SlideAnimation size_animation_;
  gfx::Rect start_bounds_;
  gfx::Rect target_bounds_;

  int left_margin_;
  int right_margin_;

  const gfx::ImageSkia* bottom_shadow_;  

  
  int outside_vertical_padding_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxPopupContentsView);
};

#endif  
