// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_STATUS_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_STATUS_BUBBLE_GTK_H_

#include <gtk/gtk.h>

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "chrome/browser/ui/status_bubble.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/point.h"
#include "url/gurl.h"

class GtkThemeService;
class Profile;

namespace gfx {
class SlideAnimation;
}

class StatusBubbleGtk : public StatusBubble,
                        public content::NotificationObserver,
                        public gfx::AnimationDelegate {
 public:
  explicit StatusBubbleGtk(Profile* profile);
  virtual ~StatusBubbleGtk();

  bool flip_horizontally() const { return flip_horizontally_; }
  int y_offset() const { return y_offset_; }

  
  virtual void SetStatus(const base::string16& status) OVERRIDE;
  virtual void SetURL(const GURL& url, const std::string& languages) OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void MouseMoved(const gfx::Point& location,
                          bool left_content) OVERRIDE;

  
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

  
  
  
  virtual void UpdateDownloadShelfVisibility(bool visible) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
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

  content::NotificationRegistrar registrar_;

  
  GtkThemeService* theme_service_;

  
  ui::OwnedWidgetGtk container_;

  
  GtkWidget* padding_;

  
  ui::OwnedWidgetGtk label_;

  
  std::string status_text_;

  
  GURL url_;

  
  std::string url_text_;

  
  
  std::string languages_;

  
  base::OneShotTimer<StatusBubbleGtk> hide_timer_;

  
  base::OneShotTimer<StatusBubbleGtk> expand_timer_;

  
  scoped_ptr<gfx::SlideAnimation> expand_animation_;

  
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
