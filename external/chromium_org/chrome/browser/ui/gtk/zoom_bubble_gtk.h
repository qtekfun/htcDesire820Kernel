// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_ZOOM_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_ZOOM_BUBBLE_GTK_H_

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/timer/timer.h"
#include "chrome/browser/ui/gtk/bubble/bubble_gtk.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"

typedef struct _GtkWidget GtkWidget;

class FullscreenController;

namespace content {
class NotificationDetails;
class NotificationSource;
class WebContents;
}

class ZoomBubbleGtk : public content::NotificationObserver {
 public:
  
  
  
  static void ShowBubble(content::WebContents* web_contents,
                         bool auto_close);

  
  static bool IsShowing();

  
  static void CloseBubble();

 private:
  ZoomBubbleGtk(GtkWidget* anchor,
                content::WebContents* web_contents,
                bool auto_close,
                FullscreenController* fullscreen_controller);

  virtual ~ZoomBubbleGtk();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void StartTimerIfNecessary();

  
  void StopTimerIfNecessary();

  
  
  void Refresh();

  
  void Close();

  
  CHROMEGTK_CALLBACK_0(ZoomBubbleGtk, void, OnDestroy);

  
  CHROMEGTK_CALLBACK_0(ZoomBubbleGtk, void, OnSetDefaultLinkClick);

  
  CHROMEGTK_CALLBACK_1(ZoomBubbleGtk, gboolean, OnMouseEnter,
                       GdkEventCrossing*);
  CHROMEGTK_CALLBACK_1(ZoomBubbleGtk, gboolean, OnMouseLeave,
                       GdkEventCrossing*);

  
  bool auto_close_;

  
  bool mouse_inside_;

  
  base::OneShotTimer<ZoomBubbleGtk> timer_;

  
  content::WebContents* web_contents_;

  
  GtkWidget* event_box_;

  
  GtkWidget* label_;

  
  BubbleGtk* bubble_;

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ZoomBubbleGtk);
};

#endif  
