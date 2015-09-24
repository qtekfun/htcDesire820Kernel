// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TABS_TAB_RENDERER_GTK_H_
#define CHROME_BROWSER_UI_GTK_TABS_TAB_RENDERER_GTK_H_

#include <gtk/gtk.h>
#include <map>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/tabs/tab_utils.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/font.h"
#include "ui/gfx/image/cairo_cached_surface.h"
#include "ui/gfx/rect.h"

namespace gfx {
class CairoCachedSurface;
class Image;
class Size;
class SlideAnimation;
class ThrobAnimation;
}  

class CustomDrawButton;
class GtkThemeService;

namespace content {
class WebContents;
}

class TabRendererGtk : public gfx::AnimationDelegate,
                       public content::NotificationObserver {
 public:
  
  enum AnimationState {
    ANIMATION_NONE,
    ANIMATION_WAITING,
    ANIMATION_LOADING
  };

  class LoadingAnimation : public content::NotificationObserver {
   public:
    struct Data {
      explicit Data(GtkThemeService* theme_service);
      Data(int loading, int waiting, int waiting_to_loading);

      int loading_animation_frame_count;
      int waiting_animation_frame_count;
      int waiting_to_loading_frame_count_ratio;
    };

    explicit LoadingAnimation(GtkThemeService* theme_service);

    
    explicit LoadingAnimation(const LoadingAnimation::Data& data);

    virtual ~LoadingAnimation();

    
    
    
    bool ValidateLoadingAnimation(AnimationState animation_state);

    AnimationState animation_state() const { return animation_state_; }
    int animation_frame() const { return animation_frame_; }

    
    virtual void Observe(int type,
                         const content::NotificationSource& source,
                         const content::NotificationDetails& details) OVERRIDE;

   private:
    scoped_ptr<Data> data_;

    
    content::NotificationRegistrar registrar_;

    
    GtkThemeService* theme_service_;

    
    AnimationState animation_state_;

    
    int animation_frame_;

    DISALLOW_COPY_AND_ASSIGN(LoadingAnimation);
  };

  explicit TabRendererGtk(GtkThemeService* theme_service);
  virtual ~TabRendererGtk();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  virtual void UpdateData(content::WebContents* contents,
                          bool app,
                          bool loading_only);

  
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

  
  virtual bool IsActive() const;

  
  void set_is_active(bool is_active) { is_active_ = is_active; }

  
  virtual bool IsSelected() const;

  
  virtual bool IsVisible() const;

  
  virtual void SetVisible(bool visible) const;

  
  
  void Paint(GtkWidget* widget, cairo_t* cr);

  
  
  cairo_surface_t* PaintToSurface(GtkWidget* widget, cairo_t* cr);

  
  
  void SchedulePaint();

  
  virtual void CloseButtonClicked();

  
  virtual void SetBounds(const gfx::Rect& bounds);

  
  
  
  bool ValidateLoadingAnimation(AnimationState animation_state);

  
  void PaintFaviconArea(GtkWidget* widget, cairo_t* cr);

  
  bool ShouldShowIcon() const;

  
  
  void MaybeAdjustLeftForMiniTab(gfx::Rect* bounds) const;

  
  static gfx::Size GetMinimumUnselectedSize();
  
  
  
  static gfx::Size GetMinimumSelectedSize();
  
  
  static gfx::Size GetStandardSize();

  
  static int GetMiniWidth();

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

  
  void Raise() const;

  
  base::string16 GetTitle() const;

  
  CHROMEGTK_CALLBACK_1(TabRendererGtk, gboolean, OnEnterNotifyEvent,
                       GdkEventCrossing*);

  
  CHROMEGTK_CALLBACK_1(TabRendererGtk, gboolean, OnLeaveNotifyEvent,
                       GdkEventCrossing*);

 private:
  class FaviconCrashAnimation;

  
  
  
  struct TabData {
    TabData();
    ~TabData();

    SkBitmap favicon;
    gfx::CairoCachedSurface cairo_favicon;
    bool is_default_favicon;
    base::string16 title;
    bool loading;
    bool crashed;
    bool incognito;
    bool show_icon;
    bool mini;
    bool blocked;
    bool animating_mini_change;
    bool app;
    TabMediaState media_state;
    TabMediaState previous_media_state;
  };

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationCanceled(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;

  
  void StartCrashAnimation();
  void StopCrashAnimation();

  
  bool IsPerformingCrashAnimation() const;

  
  
  void StartMediaIndicatorAnimation();

  
  void SetFaviconHidingOffset(int offset);

  void DisplayCrashedFavicon();
  void ResetCrashedFavicon();

  
  void Layout();

  
  
  gfx::Rect GetLocalBounds();

  
  void MoveCloseButtonWidget();

  
  static int GetContentHeight();

  void PaintTab(GtkWidget* widget, GdkEventExpose* event);

  
  void PaintTitle(GtkWidget* widget, cairo_t* cr);
  void PaintIcon(GtkWidget* widget, cairo_t* cr);
  void PaintMediaIndicator(GtkWidget* widget, cairo_t* cr);
  void PaintTabBackground(GtkWidget* widget, cairo_t* cr);
  void PaintInactiveTabBackground(GtkWidget* widget, cairo_t* cr);
  void PaintActiveTabBackground(GtkWidget* widget, cairo_t* cr);
  void PaintLoadingAnimation(GtkWidget* widget, cairo_t* cairo);

  
  
  void DrawTabBackground(cairo_t* cr,
                         GtkWidget* widget,
                         const gfx::Image& tab_bg,
                         int offset_x,
                         int offset_y);

  
  void DrawTabShadow(cairo_t* cr,
                     GtkWidget* widget,
                     int left_idr,
                     int center_idr,
                     int right_idr);

  
  
  int IconCapacity() const;

  
  bool ShouldShowMediaIndicator() const;

  
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
  gfx::Rect media_indicator_bounds_;
  gfx::Rect close_button_bounds_;

  TabData data_;

  static int tab_active_l_width_;
  static int tab_active_l_height_;
  static int tab_inactive_l_width_;
  static int tab_inactive_l_height_;

  static gfx::Font* title_font_;
  static int title_font_height_;

  static int close_button_width_;
  static int close_button_height_;

  content::NotificationRegistrar registrar_;

  
  ui::OwnedWidgetGtk tab_;

  
  
  bool showing_icon_;

  
  
  bool showing_media_indicator_;

  
  
  bool showing_close_button_;

  
  int favicon_hiding_offset_;

  
  scoped_ptr<FaviconCrashAnimation> crash_animation_;

  
  bool should_display_crashed_favicon_;

  
  gfx::Rect bounds_;

  
  
  gfx::Rect requisition_;

  
  scoped_ptr<gfx::SlideAnimation> hover_animation_;

  
  scoped_ptr<gfx::ThrobAnimation> mini_title_animation_;

  
  
  scoped_ptr<gfx::Animation> media_indicator_animation_;
  TabMediaState animating_media_state_;

  
  LoadingAnimation loading_animation_;

  
  int background_offset_x_;

  
  
  
  int background_offset_y_;

  GtkThemeService* theme_service_;

  
  scoped_ptr<CustomDrawButton> close_button_;

  
  SkColor close_button_color_;

  
  bool is_active_;

  
  SkColor selected_title_color_;

  
  SkColor unselected_title_color_;

  DISALLOW_COPY_AND_ASSIGN(TabRendererGtk);
};

#endif  
