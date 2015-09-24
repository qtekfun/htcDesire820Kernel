// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_GTK_FIRST_RUN_BUBBLE_H_
#define CHROME_BROWSER_UI_GTK_FIRST_RUN_BUBBLE_H_
#pragma once

#include <gtk/gtk.h>

#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/first_run/first_run.h"
#include "chrome/browser/ui/gtk/info_bubble_gtk.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class Profile;

class FirstRunBubble : public InfoBubbleGtkDelegate,
                       public NotificationObserver {
 public:
  
  static void Show(Profile* profile,
                   GtkWidget* anchor,
                   const gfx::Rect& rect,
                   FirstRun::BubbleType bubble_type);

  
  
  virtual void InfoBubbleClosing(InfoBubbleGtk* info_bubble,
                                 bool closed_by_escape);
  virtual bool CloseOnEscape();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  FirstRunBubble(Profile* profile,
                 GtkWidget* anchor,
                 const gfx::Rect& rect,
                 FirstRun::BubbleType bubble_type);
  virtual ~FirstRunBubble();

  
  void InitializeContentForLarge();
  void InitializeContentForOEM();
  void InitializeContentForMinimal();

  
  
  void InitializeLabels(int width_resource);

  CHROMEGTK_CALLBACK_0(FirstRunBubble, void, HandleDestroy);
  CHROMEGTK_CALLBACK_0(FirstRunBubble, void, HandleKeepButton);
  CHROMEGTK_CALLBACK_0(FirstRunBubble, void, HandleChangeButton);

  
  Profile* profile_;

  
  GtkThemeService* theme_service_;

  
  
  GtkWidget* anchor_;

  
  
  GtkWidget* content_;

  
  
  std::vector<GtkWidget*> labels_;

  InfoBubbleGtk* bubble_;

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(FirstRunBubble);
};

#endif  
