// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_TAB_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_TAB_H_

#include <list>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/views/tabs/tab_renderer_data.h"
#include "ui/base/layout.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/point.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/glow_hover_controller.h"
#include "ui/views/view.h"

class TabController;

namespace gfx {
class Animation;
class AnimationContainer;
class Font;
class LinearAnimation;
class MultiAnimation;
}
namespace views {
class ImageButton;
}

class Tab : public gfx::AnimationDelegate,
            public views::ButtonListener,
            public views::ContextMenuController,
            public views::View {
 public:
  
  static const char kViewClassName[];

  explicit Tab(TabController* controller);
  virtual ~Tab();

  TabController* controller() const { return controller_; }

  
  void set_closing(bool closing) { closing_ = closing; }
  bool closing() const { return closing_; }

  
  void set_dragging(bool dragging) { dragging_ = dragging; }
  bool dragging() const { return dragging_; }

  
  void set_animation_container(gfx::AnimationContainer* container);

  
  
  
  void set_theme_provider(ui::ThemeProvider* provider) {
    theme_provider_ = provider;
  }

  
  bool IsActive() const;

  
  bool IsSelected() const;

  
  void SetData(const TabRendererData& data);
  const TabRendererData& data() const { return data_; }

  
  
  void UpdateLoadingAnimation(TabRendererData::NetworkState state);

  
  void StartPulse();
  void StopPulse();

  
  void StartMiniTabTitleAnimation();
  void StopMiniTabTitleAnimation();

  
  
  void set_background_offset(const gfx::Point& offset) {
    background_offset_ = offset;
  }

  
  
  
  
  bool tab_activated_with_last_gesture_begin() const {
    return tab_activated_with_last_gesture_begin_;
  }

  views::GlowHoverController* hover_controller() {
    return &hover_controller_;
  }

  
  static gfx::Size GetMinimumUnselectedSize();
  
  
  
  static gfx::Size GetMinimumSelectedSize();
  
  
  static gfx::Size GetStandardSize();

  
  static int GetTouchWidth();

  
  static int GetMiniWidth();

  
  static int GetImmersiveHeight();

 private:
  friend class TabTest;
  FRIEND_TEST_ALL_PREFIXES(TabTest, CloseButtonLayout);

  friend class TabStripTest;
  FRIEND_TEST_ALL_PREFIXES(TabStripTest, TabHitTestMaskWhenStacked);
  FRIEND_TEST_ALL_PREFIXES(TabStripTest, ClippedTabCloseButton);

  
  class FaviconCrashAnimation;
  class TabCloseButton;

  
  struct ImageCacheEntry {
    ImageCacheEntry();
    ~ImageCacheEntry();

    
    int resource_id;

    
    ui::ScaleFactor scale_factor;

    
    gfx::ImageSkia image;
  };

  typedef std::list<ImageCacheEntry> ImageCache;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationCanceled(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void ShowContextMenuForView(views::View* source,
                                      const gfx::Point& point,
                                      ui::MenuSourceType source_type) OVERRIDE;

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnThemeChanged() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;
  virtual bool HasHitTestMask() const OVERRIDE;
  virtual void GetHitTestMask(HitTestSource source,
                              gfx::Path* path) const OVERRIDE;
  virtual bool GetTooltipText(const gfx::Point& p,
                              base::string16* tooltip) const OVERRIDE;
  virtual bool GetTooltipTextOrigin(const gfx::Point& p,
                                    gfx::Point* origin) const OVERRIDE;
  virtual ui::ThemeProvider* GetThemeProvider() const OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseMoved(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  const gfx::Rect& GetTitleBounds() const;
  const gfx::Rect& GetIconBounds() const;

  
  
  void MaybeAdjustLeftForMiniTab(gfx::Rect* bounds) const;

  
  void DataChanged(const TabRendererData& old);

  
  void PaintTab(gfx::Canvas* canvas);

  
  void PaintImmersiveTab(gfx::Canvas* canvas);

  
  void PaintTabBackground(gfx::Canvas* canvas);
  void PaintInactiveTabBackgroundWithTitleChange(
      gfx::Canvas* canvas,
      gfx::MultiAnimation* animation);
  void PaintInactiveTabBackground(gfx::Canvas* canvas);
  void PaintInactiveTabBackgroundUsingResourceId(gfx::Canvas* canvas,
                                                 int tab_id);
  void PaintActiveTabBackground(gfx::Canvas* canvas);

  
  void PaintIcon(gfx::Canvas* canvas);
  void PaintMediaIndicator(gfx::Canvas* canvas);
  void PaintTitle(gfx::Canvas* canvas, SkColor title_color);

  
  void AdvanceLoadingAnimation(TabRendererData::NetworkState old_state,
                               TabRendererData::NetworkState state);

  
  
  int IconCapacity() const;

  
  bool ShouldShowIcon() const;

  
  bool ShouldShowMediaIndicator() const;

  
  bool ShouldShowCloseBox() const;

  
  
  
  double GetThrobValue();

  
  
  void SetFaviconHidingOffset(int offset);

  void DisplayCrashedFavicon();
  void ResetCrashedFavicon();

  void StopCrashAnimation();
  void StartCrashAnimation();

  
  bool IsPerformingCrashAnimation() const;

  
  
  void StartMediaIndicatorAnimation();

  
  void ScheduleIconPaint();

  
  gfx::Rect GetImmersiveBarRect() const;

  
  void GetTabIdAndFrameId(views::Widget* widget,
                          int* tab_id,
                          int* frame_id) const;

  
  static void InitTabResources();

  
  
  static gfx::Size GetBasicMinimumUnselectedSize();

  
  static void LoadTabImages();

  
  
  static gfx::ImageSkia GetCachedImage(int resource_id,
                                       const gfx::Size& size,
                                       ui::ScaleFactor scale_factor);

  
  static void SetCachedImage(int resource_id,
                             ui::ScaleFactor scale_factor,
                             const gfx::ImageSkia& image);

  
  
  TabController* controller_;

  TabRendererData data_;

  
  bool closing_;

  
  bool dragging_;

  
  
  int favicon_hiding_offset_;

  
  
  int loading_animation_frame_;

  
  int immersive_loading_step_;

  bool should_display_crashed_favicon_;

  
  scoped_ptr<gfx::Animation> tab_animation_;

  
  scoped_ptr<gfx::LinearAnimation> crash_icon_animation_;

  
  
  scoped_ptr<gfx::Animation> media_indicator_animation_;
  TabMediaState animating_media_state_;

  scoped_refptr<gfx::AnimationContainer> animation_container_;

  views::ImageButton* close_button_;

  ui::ThemeProvider* theme_provider_;

  bool tab_activated_with_last_gesture_begin_;

  views::GlowHoverController hover_controller_;

  
  gfx::Rect favicon_bounds_;
  gfx::Rect title_bounds_;
  gfx::Rect media_indicator_bounds_;

  
  gfx::Point background_offset_;

  struct TabImage {
    gfx::ImageSkia* image_l;
    gfx::ImageSkia* image_c;
    gfx::ImageSkia* image_r;
    int l_width;
    int r_width;
  };
  static TabImage tab_active_;
  static TabImage tab_inactive_;
  static TabImage tab_alpha_;

  
  
  bool showing_icon_;

  
  
  bool showing_media_indicator_;

  
  
  bool showing_close_button_;

  
  SkColor close_button_color_;

  static gfx::Font* font_;
  static int font_height_;

  
  
  static ImageCache* image_cache_;

  DISALLOW_COPY_AND_ASSIGN(Tab);
};

#endif  
