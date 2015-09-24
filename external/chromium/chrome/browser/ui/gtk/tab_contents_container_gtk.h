// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_TAB_CONTENTS_CONTAINER_GTK_H_
#define CHROME_BROWSER_UI_GTK_TAB_CONTENTS_CONTAINER_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "chrome/browser/ui/gtk/view_id_util.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"

class RenderViewHost;
class StatusBubbleGtk;
class TabContents;
class TabContentsWrapper;

typedef struct _GtkFloatingContainer GtkFloatingContainer;

class TabContentsContainerGtk : public NotificationObserver,
                                public ViewIDUtil::Delegate {
 public:
  explicit TabContentsContainerGtk(StatusBubbleGtk* status_bubble);
  ~TabContentsContainerGtk();

  void Init();

  
  void SetTab(TabContentsWrapper* tab);
  TabContentsWrapper* tab() const { return tab_; }

  
  
  TabContents* GetVisibleTabContents();

  void SetPreview(TabContentsWrapper* preview);
  void PopPreview();

  
  void DetachTab(TabContentsWrapper* tab);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  GtkWidget* widget() { return floating_.get(); }

  
  virtual GtkWidget* GetWidgetForViewID(ViewID id);

 private:
  
  
  
  void TabContentsDestroyed(TabContents* contents);

  
  
  static void OnSetFloatingPosition(
      GtkFloatingContainer* container, GtkAllocation* allocation,
      TabContentsContainerGtk* tab_contents_container);

  
  void PackTab(TabContentsWrapper* );

  
  void HideTab(TabContentsWrapper* tab);

  
  void RemovePreview();

  
  
  CHROMEGTK_CALLBACK_1(TabContentsContainerGtk, gboolean, OnFocus,
                       GtkDirectionType);

  NotificationRegistrar registrar_;

  
  
  TabContentsWrapper* tab_;

  
  
  TabContentsWrapper* preview_;

  
  StatusBubbleGtk* status_bubble_;

  
  
  
  
  OwnedWidgetGtk floating_;

  
  
  
  
  GtkWidget* expanded_;

  DISALLOW_COPY_AND_ASSIGN(TabContentsContainerGtk);
};

#endif  
