// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_THEME_INSTALL_BUBBLE_VIEW_GTK_H_
#define CHROME_BROWSER_UI_GTK_THEME_INSTALL_BUBBLE_VIEW_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"

class ThemeInstallBubbleViewGtk : public NotificationObserver {
 public:
  static void Show(GtkWindow* parent);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  explicit ThemeInstallBubbleViewGtk(GtkWidget* parent);

  virtual ~ThemeInstallBubbleViewGtk();

  void increment_num_loading() { num_loads_extant_++; }

  
  void InitWidgets();

  
  void MoveWindow();

  
  CHROMEGTK_CALLBACK_0(ThemeInstallBubbleViewGtk, gboolean, OnUnmapEvent);

  
  
  CHROMEGTK_CALLBACK_1(ThemeInstallBubbleViewGtk, gboolean,
                       OnExpose, GdkEventExpose*);

  GtkWidget* widget_;

  
  GtkWidget* parent_;

  
  int num_loads_extant_;

  NotificationRegistrar registrar_;

  
  static ThemeInstallBubbleViewGtk* instance_;

  DISALLOW_COPY_AND_ASSIGN(ThemeInstallBubbleViewGtk);
};

#endif  
