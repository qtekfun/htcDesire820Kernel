// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FULLSCREEN_EXIT_BUBBLE_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_FULLSCREEN_EXIT_BUBBLE_VIEWS_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/fullscreen/fullscreen_exit_bubble.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/views/widget/widget_observer.h"

class BrowserView;
class GURL;
namespace gfx {
class SlideAnimation;
}
namespace views {
class View;
class Widget;
}

class FullscreenExitBubbleViews : public FullscreenExitBubble,
                                  public content::NotificationObserver,
                                  public views::WidgetObserver {
 public:
  FullscreenExitBubbleViews(BrowserView* browser,
                            const GURL& url,
                            FullscreenExitBubbleType bubble_type);
  virtual ~FullscreenExitBubbleViews();

  void UpdateContent(const GURL& url, FullscreenExitBubbleType bubble_type);

  
  void RepositionIfVisible();

 private:
  class FullscreenExitView;

  enum AnimatedAttribute {
    ANIMATED_ATTRIBUTE_BOUNDS,
    ANIMATED_ATTRIBUTE_OPACITY
  };

  
  
  void UpdateMouseWatcher();

  
  
  void UpdateForImmersiveState();

  
  void UpdateBounds();

  
  views::View* GetBrowserRootView() const;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual gfx::Rect GetPopupRect(bool ignore_animation_state) const OVERRIDE;
  virtual gfx::Point GetCursorScreenPoint() OVERRIDE;
  virtual bool WindowContainsPoint(gfx::Point pos) OVERRIDE;
  virtual bool IsWindowActive() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual bool IsAnimating() OVERRIDE;
  virtual bool CanMouseTriggerSlideIn() const OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnWidgetVisibilityChanged(views::Widget* widget,
                                         bool visible) OVERRIDE;

  BrowserView* browser_view_;

  views::Widget* popup_;

  
  scoped_ptr<gfx::SlideAnimation> animation_;

  
  AnimatedAttribute animated_attribute_;

  
  FullscreenExitView* view_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(FullscreenExitBubbleViews);
};

#endif  
