// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_BASE_TAB_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_BASE_TAB_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/views/tabs/tab_renderer_data.h"
#include "ui/base/animation/animation_delegate.h"
#include "views/controls/button/button.h"
#include "views/view.h"

class BaseTab;
class TabController;

namespace gfx {
class Font;
}

namespace ui {
class AnimationContainer;
class SlideAnimation;
class ThrobAnimation;
}

namespace views {
class ImageButton;
}

class BaseTab : public ui::AnimationDelegate,
                public views::ButtonListener,
                public views::ContextMenuController,
                public views::View {
 public:
  explicit BaseTab(TabController* controller);
  virtual ~BaseTab();

  
  
  void SetData(const TabRendererData& data);
  const TabRendererData& data() const { return data_; }

  
  
  virtual void UpdateLoadingAnimation(TabRendererData::NetworkState state);

  
  void StartPulse();
  void StopPulse();

  
  void set_closing(bool closing) { closing_ = closing; }
  bool closing() const { return closing_; }

  
  void set_dragging(bool dragging) { dragging_ = dragging; }
  bool dragging() const { return dragging_; }

  
  void set_animation_container(ui::AnimationContainer* container);
  ui::AnimationContainer* animation_container() const {
    return animation_container_.get();
  }

  
  
  
  void set_theme_provider(ui::ThemeProvider* provider) {
    theme_provider_ = provider;
  }

  
  bool IsCloseable() const;

  
  bool IsActive() const;

  
  virtual bool IsSelected() const;

  
  virtual ThemeProvider* GetThemeProvider() const OVERRIDE;
  virtual bool OnMousePressed(const views::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnMouseEntered(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const views::MouseEvent& event) OVERRIDE;
  virtual bool GetTooltipText(const gfx::Point& p,
                              std::wstring* tooltip) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

 protected:
  
  virtual void DataChanged(const TabRendererData& old) {}

  
  virtual void AdvanceLoadingAnimation(TabRendererData::NetworkState old_state,
                                       TabRendererData::NetworkState state);

  TabController* controller() const { return controller_; }

  
  
  ui::ThrobAnimation* pulse_animation() const { return pulse_animation_.get(); }

  
  const ui::SlideAnimation* hover_animation() const {
    return hover_animation_.get();
  }

  views::ImageButton* close_button() const { return close_button_; }

  
  void PaintIcon(gfx::Canvas* canvas);
  void PaintTitle(gfx::Canvas* canvas, SkColor title_color);

  
  virtual void AnimationProgressed(const ui::Animation* animation) OVERRIDE;
  virtual void AnimationCanceled(const ui::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const ui::Animation* animation) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const views::Event& event) OVERRIDE;

  
  virtual void ShowContextMenuForView(views::View* source,
                                      const gfx::Point& p,
                                      bool is_mouse_gesture) OVERRIDE;

  
  virtual const gfx::Rect& GetTitleBounds() const = 0;
  virtual const gfx::Rect& GetIconBounds() const = 0;

  virtual int loading_animation_frame() const;
  virtual bool should_display_crashed_favicon() const;
  virtual int favicon_hiding_offset() const;

  static gfx::Font* font() { return font_; }
  static int font_height() { return font_height_; }

 private:
  
  class FaviconCrashAnimation;

  
  
  void SetFaviconHidingOffset(int offset);

  void DisplayCrashedFavicon();
  void ResetCrashedFavicon();

  
  void StartCrashAnimation();
  void StopCrashAnimation();

  
  bool IsPerformingCrashAnimation() const;

  
  void ScheduleIconPaint();

  static void InitResources();

  
  
  TabController* controller_;

  TabRendererData data_;

  
  bool closing_;

  
  bool dragging_;

  
  
  int favicon_hiding_offset_;

  
  int loading_animation_frame_;

  bool should_display_crashed_favicon_;

  
  scoped_ptr<ui::ThrobAnimation> pulse_animation_;

  
  scoped_ptr<ui::SlideAnimation> hover_animation_;

  
  scoped_ptr<FaviconCrashAnimation> crash_animation_;

  scoped_refptr<ui::AnimationContainer> animation_container_;

  views::ImageButton* close_button_;

  
  
  
  bool throbber_disabled_;

  ui::ThemeProvider* theme_provider_;

  static gfx::Font* font_;
  static int font_height_;

  DISALLOW_COPY_AND_ASSIGN(BaseTab);
};

#endif  
