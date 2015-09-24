// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TABS_TAB_RENDERER_GTK_H_
#define CHROME_BROWSER_UI_GTK_TABS_TAB_RENDERER_GTK_H_
#pragma once

#include <gtk/gtk.h>
#include <map>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/font.h"
#include "ui/gfx/rect.h"

namespace gfx {
class Size;
}  

class CustomDrawButton;
class GtkThemeService;
class TabContents;

namespace ui {
class SlideAnimation;
class ThemeProvider;
class ThrobAnimation;
}

class TabRendererGtk : public ui::AnimationDelegate,
                       public NotificationObserver {
 public:
  
  enum AnimationState {
    ANIMATION_NONE,
    ANIMATION_WAITING,
    ANIMATION_LOADING
  };

  class LoadingAnimation : public NotificationObserver {
   public:
    struct Data {
      explicit Data(ui::ThemeProvider* theme_provider);
      Data(int loading, int waiting, int waiting_to_loading);

      SkBitmap* waiting_animation_frames;
      SkBitmap* loading_animation_frames;
      int loading_animation_frame_count;
      int waiting_animation_frame_count;
      int waiting_to_loading_frame_count_ratio;
    };

    explicit LoadingAnimation(ui::ThemeProvider* theme_provider);

    
    explicit LoadingAnimation(const LoadingAnimation::Data& data);

    virtual ~LoadingAnimation();

    
    
    
    bool ValidateLoadingAnimation(AnimationState animation_state);

    AnimationState animation_state() const { return animation_state_; }
    int animation_frame() const { return animation_frame_; }

    const SkBitmap* waiting_animation_frames() const {
      return data_->waiting_animation_frames;
    }
    const SkBitmap* loading_animation_frames() const {
      return data_->loading_animation_frames;
    }

    
    virtual void Observe(NotificationType type,
                         const NotificationSource& source,
                         const NotificationDetails& details);

   private:
    scoped_ptr<Data> data_;

    
    NotificationRegistrar registrar_;

    
    ui::ThemeProvider* theme_service_;

    
    AnimationState animation_state_;

    
    int animation_frame_;

    DISALLOW_COPY_AND_ASSIGN(LoadingAnimation);
  };

  explicit TabRendererGtk(ui::ThemeProvider* theme_provider);
  virtual ~TabRendererGtk();

  
  
  
  virtual void UpdateData(TabContents* contents, bool app, bool loading_only);

  
  void SetBlocked(bool pinned);
  bool is_blocked() const;

  
  void set_mini(bool mini) { data_.mini = mini; }
  bool mini() const { return data_.mini; }

  
  void set_app(bool app) { data_.app = app; }
  bool app() const { return data_.app; }

  
  void set_animating_mini_change(bool value) {
    data_.animating_mini_change = value;
  }

  
  void UpdateFromModel();

  
  virtual bool IsSelected() const;

  
  virtual bool IsVisible() const;

  
  virtual void SetVisible(bool visible) const;

  
  virtual void Paint(gfx::Canvas* canvas);

  
  virtual SkBitmap PaintBitmap();

  
  
  virtual cairo_surface_t* PaintToSurface();

  
  
  virtual void SchedulePaint();

  
  virtual void CloseButtonClicked();

  
  virtual void SetBounds(const gfx::Rect& bounds);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  bool ValidateLoadingAnimation(AnimationState animation_state);

  
  void PaintFaviconArea(GdkEventExpose* event);

  
  bool ShouldShowIcon() const;

  
  static gfx::Size GetMinimumUnselectedSize();
  
  
  
  static gfx::Size GetMinimumSelectedSize();
  
  
  static gfx::Size GetStandardSize();

  
  static int GetMiniWidth();

  
  static void LoadTabImages();

  
  static void SetSelectedTitleColor(SkColor color);
  static void SetUnselectedTitleColor(SkColor color);

  static gfx::Font* title_font() { return title_font_; }

  
  int x() const { return bounds_.x(); }
  int y() const { return bounds_.y(); }
  int width() const { return bounds_.width(); }
  int height() const { return bounds_.height(); }

  gfx::Rect bounds() const { return bounds_; }

  gfx::Rect favicon_bounds() const { return favicon_bounds_; }

  
  gfx::Rect GetNonMirroredBounds(GtkWidget* parent) const;

  
  gfx::Rect GetRequisition() const;

  GtkWidget* widget() const { return tab_.get(); }

  
  void StartMiniTabTitleAnimation();
  void StopMiniTabTitleAnimation();

  void set_vertical_offset(int offset) { background_offset_y_ = offset; }

 protected:
  const gfx::Rect& title_bounds() const { return title_bounds_; }
  const gfx::Rect& close_button_bounds() const { return close_button_bounds_; }

  
  string16 GetTitle() const;

  
  CHROMEGTK_CALLBACK_1(TabRendererGtk, gboolean, OnEnterNotifyEvent,
                       GdkEventCrossing*);

  
  CHROMEGTK_CALLBACK_1(TabRendererGtk, gboolean, OnLeaveNotifyEvent,
                       GdkEventCrossing*);

 private:
  class FaviconCrashAnimation;

  
  
  
  struct CachedBitmap {
    int bg_offset_x;
    int bg_offset_y;
    SkBitmap* bitmap;
  };
  typedef std::map<std::pair<const SkBitmap*, const SkBitmap*>, CachedBitmap>
      BitmapCache;

  
  
  
  struct TabData {
    TabData()
        : is_default_favicon(false),
          loading(false),
          crashed(false),
          incognito(false),
          show_icon(true),
          mini(false),
          blocked(false),
          animating_mini_change(false),
          app(false) {
    }

    SkBitmap favicon;
    bool is_default_favicon;
    string16 title;
    bool loading;
    bool crashed;
    bool incognito;
    bool show_icon;
    bool mini;
    bool blocked;
    bool animating_mini_change;
    bool app;
  };

  
  struct TabImage {
    SkBitmap* image_l;
    SkBitmap* image_c;
    SkBitmap* image_r;
    int l_width;
    int r_width;
    int y_offset;
  };

  
  virtual void AnimationProgressed(const ui::Animation* animation);
  virtual void AnimationCanceled(const ui::Animation* animation);
  virtual void AnimationEnded(const ui::Animation* animation);

  
  void StartCrashAnimation();
  void StopCrashAnimation();

  
  bool IsPerformingCrashAnimation() const;

  
  void SetFaviconHidingOffset(int offset);

  void DisplayCrashedFavicon();
  void ResetCrashedFavicon();

  
  void Layout();

  
  
  gfx::Rect GetLocalBounds();

  
  void MoveCloseButtonWidget();

  
  static int GetContentHeight();

  
  
  
  SkBitmap* GetMaskedBitmap(const SkBitmap* mask,
                            const SkBitmap* background,
                            int bg_offset_x,
                            int bg_offset_y);
  BitmapCache cached_bitmaps_;

  
  void PaintTab(GdkEventExpose* event);

  
  void PaintTitle(gfx::Canvas* canvas);
  void PaintIcon(gfx::Canvas* canvas);
  void PaintTabBackground(gfx::Canvas* canvas);
  void PaintInactiveTabBackground(gfx::Canvas* canvas);
  void PaintActiveTabBackground(gfx::Canvas* canvas);
  void PaintLoadingAnimation(gfx::Canvas* canvas);

  
  
  int IconCapacity() const;


  
  bool ShouldShowCloseBox() const;

  CustomDrawButton* MakeCloseButton();

  
  
  
  double GetThrobValue();

  
  CHROMEGTK_CALLBACK_0(TabRendererGtk, void, OnCloseButtonClicked);

  
  CHROMEGTK_CALLBACK_1(TabRendererGtk, gboolean, OnCloseButtonMouseRelease,
                       GdkEventButton*);

  
  CHROMEGTK_CALLBACK_1(TabRendererGtk, gboolean, OnExposeEvent,
                       GdkEventExpose*);

  
  CHROMEGTK_CALLBACK_1(TabRendererGtk, void, OnSizeAllocate, GtkAllocation*);

  
  static void InitResources();
  static bool initialized_;

  
  gfx::Rect favicon_bounds_;
  gfx::Rect title_bounds_;
  gfx::Rect close_button_bounds_;

  TabData data_;

  static TabImage tab_active_;
  static TabImage tab_inactive_;
  static TabImage tab_alpha_;

  static gfx::Font* title_font_;
  static int title_font_height_;

  static int close_button_width_;
  static int close_button_height_;

  static SkColor selected_title_color_;
  static SkColor unselected_title_color_;

  
  OwnedWidgetGtk tab_;

  
  
  bool showing_icon_;

  
  
  bool showing_close_button_;

  
  int favicon_hiding_offset_;

  
  scoped_ptr<FaviconCrashAnimation> crash_animation_;

  
  bool should_display_crashed_favicon_;

  
  gfx::Rect bounds_;

  
  
  gfx::Rect requisition_;

  
  scoped_ptr<ui::SlideAnimation> hover_animation_;

  
  scoped_ptr<ui::ThrobAnimation> mini_title_animation_;

  
  LoadingAnimation loading_animation_;

  
  int background_offset_x_;

  
  
  
  int background_offset_y_;

  GtkThemeService* theme_service_;

  
  scoped_ptr<CustomDrawButton> close_button_;

  
  SkColor close_button_color_;

  
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(TabRendererGtk);
};

#endif  
