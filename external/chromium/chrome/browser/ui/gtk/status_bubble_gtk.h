// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_STATUS_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_STATUS_BUBBLE_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/timer.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "chrome/browser/ui/status_bubble.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "googleurl/src/gurl.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/gfx/point.h"

class GtkThemeService;
class Profile;

namespace ui {
class SlideAnimation;
}

class StatusBubbleGtk : public StatusBubble,
                        public NotificationObserver,
                        public ui::AnimationDelegate {
 public:
  explicit StatusBubbleGtk(Profile* profile);
  virtual ~StatusBubbleGtk();

  bool flip_horizontally() const { return flip_horizontally_; }
  int y_offset() const { return y_offset_; }

  
  virtual void SetStatus(const string16& status);
  virtual void SetURL(const GURL& url, const string16& languages);
  virtual void Hide();
  virtual void MouseMoved(const gfx::Point& location, bool left_content);

  
  virtual void AnimationEnded(const ui::Animation* animation);
  virtual void AnimationProgressed(const ui::Animation* animation);

  
  
  
  virtual void UpdateDownloadShelfVisibility(bool visible);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  GtkWidget* widget() { return container_.get(); }

 private:
  
  
  void SetStatusTextTo(const std::string& status_utf8);

  
  
  void SetStatusTextToURL();

  
  
  void Show();

  
  void InitWidgets();

  
  void UserChangedTheme();

  
  
  
  void SetFlipHorizontally(bool flip_horizontally);

  
  
  
  void ExpandURL();

  
  void UpdateLabelSizeRequest();

  
  
  bool expanded() {
    return expand_animation_.get();
  }

  CHROMEGTK_CALLBACK_1(StatusBubbleGtk, gboolean, HandleMotionNotify,
                       GdkEventMotion*);

  CHROMEGTK_CALLBACK_1(StatusBubbleGtk, gboolean, HandleEnterNotify,
                       GdkEventCrossing*);

  NotificationRegistrar registrar_;

  
  GtkThemeService* theme_service_;

  
  OwnedWidgetGtk container_;

  
  GtkWidget* padding_;

  
  OwnedWidgetGtk label_;

  
  std::string status_text_;

  
  GURL url_;

  
  std::string url_text_;

  
  
  string16 languages_;

  
  base::OneShotTimer<StatusBubbleGtk> hide_timer_;

  
  base::OneShotTimer<StatusBubbleGtk> expand_timer_;

  
  scoped_ptr<ui::SlideAnimation> expand_animation_;

  
  int start_width_;
  int desired_width_;

  
  
  
  bool flip_horizontally_;

  
  int y_offset_;

  
  bool download_shelf_is_visible_;

  
  
  
  gfx::Point last_mouse_location_;
  bool last_mouse_left_content_;

  
  
  bool ignore_next_left_content_;
};

#endif  
