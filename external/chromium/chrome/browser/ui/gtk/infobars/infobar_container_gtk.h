// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_INFOBARS_INFOBAR_CONTAINER_GTK_H_
#define CHROME_BROWSER_UI_GTK_INFOBARS_INFOBAR_CONTAINER_GTK_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class InfoBar;
class InfoBarDelegate;
class Profile;
class TabContents;

typedef struct _GtkWidget GtkWidget;

class InfoBarContainerGtk : public NotificationObserver {
 public:
  explicit InfoBarContainerGtk(Profile* profile);
  virtual ~InfoBarContainerGtk();

  
  GtkWidget* widget() const { return container_.get(); }

  
  
  
  void ChangeTabContents(TabContents* contents);

  
  
  
  void RemoveDelegate(InfoBarDelegate* delegate);

  
  
  int TotalHeightOfAnimatingBars() const;

 private:
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  void UpdateInfoBars();

  
  
  void ShowArrowForDelegate(InfoBarDelegate* delegate, bool animate);

  
  
  void AddInfoBar(InfoBarDelegate* delegate, bool animate);

  
  
  
  void RemoveInfoBar(InfoBarDelegate* delegate, bool animate);

  
  
  void UpdateToolbarInfoBarState(InfoBar* infobar, bool animate);

  NotificationRegistrar registrar_;

  
  Profile* profile_;

  
  TabContents* tab_contents_;

  
  OwnedWidgetGtk container_;

  DISALLOW_COPY_AND_ASSIGN(InfoBarContainerGtk);
};

#endif  
