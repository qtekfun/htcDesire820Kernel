// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FRAME_GLASS_BROWSER_FRAME_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_FRAME_GLASS_BROWSER_FRAME_VIEW_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/prefs/pref_member.h"
#include "chrome/browser/ui/views/frame/browser_frame_win.h"
#include "chrome/browser/ui/views/frame/browser_non_client_frame_view.h"
#include "views/controls/button/button.h"
#include "views/controls/menu/view_menu_delegate.h"
#include "views/window/non_client_view.h"

class BrowserView;
class SkBitmap;

namespace views {
class ProfileMenuButton;
class ProfileMenuModel;
class ProfileTagView;
}

class GlassBrowserFrameView : public BrowserNonClientFrameView,
                              public NotificationObserver,
                              public views::ViewMenuDelegate {
 public:
  
  GlassBrowserFrameView(BrowserFrame* frame, BrowserView* browser_view);
  virtual ~GlassBrowserFrameView();

  
  virtual gfx::Rect GetBoundsForTabStrip(views::View* tabstrip) const OVERRIDE;
  virtual int GetHorizontalTabStripVerticalOffset(bool restored) const OVERRIDE;
  virtual void UpdateThrobber(bool running) OVERRIDE;

  
  virtual gfx::Rect GetBoundsForClientView() const OVERRIDE;
  virtual bool AlwaysUseNativeFrame() const OVERRIDE;
  virtual gfx::Rect GetWindowBoundsForClientBounds(
      const gfx::Rect& client_bounds) const OVERRIDE;
  virtual int NonClientHitTest(const gfx::Point& point) OVERRIDE;
  virtual void GetWindowMask(const gfx::Size& size, gfx::Path* window_mask)
      OVERRIDE { }
  virtual void EnableClose(bool enable) OVERRIDE { }
  virtual void ResetWindowControls() OVERRIDE { }
  virtual void UpdateWindowIcon() OVERRIDE { }

  
  virtual void RunMenu(views::View* source, const gfx::Point& pt) OVERRIDE;

 protected:
  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual bool HitTest(const gfx::Point& l) const OVERRIDE;

 private:
  
  
  int FrameBorderThickness() const;

  
  
  int NonClientBorderThickness() const;

  
  
  
  
  
  int NonClientTopBorderHeight(bool restored, bool ignore_vertical_tabs) const;

  
  void PaintToolbarBackground(gfx::Canvas* canvas);
  void PaintOTRAvatar(gfx::Canvas* canvas);
  void PaintRestoredClientEdge(gfx::Canvas* canvas);

  
  void LayoutOTRAvatar();
  void LayoutClientView();
  void LayoutProfileTag();

  
  gfx::Rect CalculateClientAreaBounds(int width, int height) const;

  
  void StartThrobber();
  void StopThrobber();

  
  void DisplayNextThrobberFrame();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  void RegisterLoginNotifications();

  
  bool show_profile_button() const { return profile_button_.get() != NULL; }

  
  gfx::Rect otr_avatar_bounds_;

  
  BrowserFrame* frame_;

  
  BrowserView* browser_view_;

  
  gfx::Rect client_view_bounds_;

  
  scoped_ptr<views::ProfileMenuButton> profile_button_;

  
  scoped_ptr<views::ProfileTagView> profile_tag_;

  
  scoped_ptr<views::ProfileMenuModel> profile_menu_model_;

  
  bool throbber_running_;

  
  int throbber_frame_;

  
  StringPrefMember username_pref_;

  static const int kThrobberIconCount = 24;
  static HICON throbber_icons_[kThrobberIconCount];
  static void InitThrobberIcons();

  DISALLOW_COPY_AND_ASSIGN(GlassBrowserFrameView);
};

#endif  
