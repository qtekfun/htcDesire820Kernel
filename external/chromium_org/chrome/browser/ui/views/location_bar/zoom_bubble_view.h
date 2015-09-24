// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_ZOOM_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_ZOOM_BUBBLE_VIEW_H_

#include "base/basictypes.h"
#include "base/timer/timer.h"
#include "chrome/browser/ui/views/frame/immersive_mode_controller.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/label.h"

class FullscreenController;

namespace content {
class NotificationDetails;
class NotificationSource;
class WebContents;
}

class ZoomBubbleView : public views::BubbleDelegateView,
                       public views::ButtonListener,
                       public content::NotificationObserver,
                       public ImmersiveModeController::Observer {
 public:
  
  
  static void ShowBubble(content::WebContents* web_contents,
                         bool auto_close);

  
  static void CloseBubble();

  
  static bool IsShowing();

  
  
  static const ZoomBubbleView* GetZoomBubbleForTest();

 private:
  ZoomBubbleView(views::View* anchor_view,
                 content::WebContents* web_contents,
                 bool auto_close,
                 ImmersiveModeController* immersive_mode_controller,
                 FullscreenController* fullscreen_controller);
  virtual ~ZoomBubbleView();

  
  
  
  
  void AdjustForFullscreen(const gfx::Rect& screen_bounds);

  
  
  void Refresh();

  void Close();

  
  void StartTimerIfNecessary();

  
  void StopTimer();

  
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void Init() OVERRIDE;
  virtual void WindowClosing() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnImmersiveRevealStarted() OVERRIDE;
  virtual void OnImmersiveModeControllerDestroyed() OVERRIDE;

  
  
  
  static ZoomBubbleView* zoom_bubble_;

  
  base::OneShotTimer<ZoomBubbleView> timer_;

  
  views::Label* label_;

  
  content::WebContents* web_contents_;

  
  bool auto_close_;

  
  
  
  ImmersiveModeController* immersive_mode_controller_;

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ZoomBubbleView);
};

#endif  
