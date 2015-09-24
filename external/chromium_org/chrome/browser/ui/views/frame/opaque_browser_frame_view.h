// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_OPAQUE_BROWSER_FRAME_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_OPAQUE_BROWSER_FRAME_VIEW_H_

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/view_ids.h"
#include "chrome/browser/ui/views/frame/browser_frame.h"
#include "chrome/browser/ui/views/frame/browser_non_client_frame_view.h"
#include "chrome/browser/ui/views/frame/opaque_browser_frame_view_layout_delegate.h"
#include "chrome/browser/ui/views/tab_icon_view_model.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/window/non_client_view.h"

class BrowserView;
class OpaqueBrowserFrameViewLayout;
class OpaqueBrowserFrameViewPlatformSpecific;
class TabIconView;
class NewAvatarButton;

namespace views {
class ImageButton;
class FrameBackground;
class Label;
}

class OpaqueBrowserFrameView : public BrowserNonClientFrameView,
                               public content::NotificationObserver,
                               public views::ButtonListener,
                               public chrome::TabIconViewModel,
                               public OpaqueBrowserFrameViewLayoutDelegate {
 public:
  
  OpaqueBrowserFrameView(BrowserFrame* frame, BrowserView* browser_view);
  virtual ~OpaqueBrowserFrameView();

  
  virtual gfx::Rect GetBoundsForTabStrip(views::View* tabstrip) const OVERRIDE;
  virtual int GetTopInset() const OVERRIDE;
  virtual int GetThemeBackgroundXInset() const OVERRIDE;
  virtual void UpdateThrobber(bool running) OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;

  
  virtual gfx::Rect GetBoundsForClientView() const OVERRIDE;
  virtual gfx::Rect GetWindowBoundsForClientBounds(
      const gfx::Rect& client_bounds) const OVERRIDE;
  virtual int NonClientHitTest(const gfx::Point& point) OVERRIDE;
  virtual void GetWindowMask(const gfx::Size& size, gfx::Path* window_mask)
      OVERRIDE;
  virtual void ResetWindowControls() OVERRIDE;
  virtual void UpdateWindowIcon() OVERRIDE;
  virtual void UpdateWindowTitle() OVERRIDE;

  
  virtual bool HitTestRect(const gfx::Rect& rect) const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender, const ui::Event& event)
      OVERRIDE;

  
  virtual bool ShouldTabIconViewAnimate() const OVERRIDE;
  virtual gfx::ImageSkia GetFaviconForTabIconView() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual bool ShouldShowWindowIcon() const OVERRIDE;
  virtual bool ShouldShowWindowTitle() const OVERRIDE;
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual int GetIconSize() const OVERRIDE;
  virtual bool ShouldLeaveOffsetNearTopBorder() const OVERRIDE;
  virtual gfx::Size GetBrowserViewMinimumSize() const OVERRIDE;
  virtual bool ShouldShowCaptionButtons() const OVERRIDE;
  virtual bool ShouldShowAvatar() const OVERRIDE;
  virtual bool IsRegularOrGuestSession() const OVERRIDE;
  virtual gfx::ImageSkia GetOTRAvatarIcon() const OVERRIDE;
  virtual bool IsMaximized() const OVERRIDE;
  virtual bool IsMinimized() const OVERRIDE;
  virtual bool IsFullscreen() const OVERRIDE;
  virtual bool IsTabStripVisible() const OVERRIDE;
  virtual int GetTabStripHeight() const OVERRIDE;
  virtual int GetAdditionalReservedSpaceInTabStrip() const OVERRIDE;
  virtual gfx::Size GetTabstripPreferredSize() const OVERRIDE;

 protected:
  views::ImageButton* minimize_button() const { return minimize_button_; }
  views::ImageButton* maximize_button() const { return maximize_button_; }
  views::ImageButton* restore_button() const { return restore_button_; }
  views::ImageButton* close_button() const { return close_button_; }

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

 private:
  
  
  views::ImageButton* InitWindowCaptionButton(int normal_image_id,
                                              int hot_image_id,
                                              int pushed_image_id,
                                              int mask_image_id,
                                              int accessibility_string_id,
                                              ViewID view_id);

  
  
  
  int FrameBorderThickness(bool restored) const;

  
  
  int TopResizeHeight() const;

  
  
  int NonClientBorderThickness() const;

  
  
  gfx::Rect IconBounds() const;

  
  
  
  void PaintRestoredFrameBorder(gfx::Canvas* canvas);
  void PaintMaximizedFrameBorder(gfx::Canvas* canvas);
  void PaintToolbarBackground(gfx::Canvas* canvas);
  void PaintRestoredClientEdge(gfx::Canvas* canvas);

  
  SkColor GetFrameColor() const;
  gfx::ImageSkia* GetFrameImage() const;
  gfx::ImageSkia* GetFrameOverlayImage() const;
  int GetTopAreaHeight() const;

  
  gfx::Rect CalculateClientAreaBounds(int width, int height) const;

  
  OpaqueBrowserFrameViewLayout* layout_;

  
  views::ImageButton* minimize_button_;
  views::ImageButton* maximize_button_;
  views::ImageButton* restore_button_;
  views::ImageButton* close_button_;

  
  TabIconView* window_icon_;
  views::Label* window_title_;

  content::NotificationRegistrar registrar_;

  
  scoped_ptr<views::FrameBackground> frame_background_;

  
  scoped_ptr<OpaqueBrowserFrameViewPlatformSpecific> platform_observer_;

  DISALLOW_COPY_AND_ASSIGN(OpaqueBrowserFrameView);
};

#endif  
