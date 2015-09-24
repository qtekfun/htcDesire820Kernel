// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_DOWNLOAD_DOWNLOAD_SHELF_GTK_H_
#define CHROME_BROWSER_UI_GTK_DOWNLOAD_DOWNLOAD_SHELF_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "chrome/browser/download/download_shelf.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "chrome/browser/ui/gtk/slide_animator_gtk.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/gfx/native_widget_types.h"

class BaseDownloadItemModel;
class Browser;
class CustomDrawButton;
class DownloadItemGtk;
class GtkThemeService;
class SlideAnimatorGtk;

namespace gfx {
class Point;
}

class DownloadShelfGtk : public DownloadShelf,
                         public NotificationObserver,
                         public SlideAnimatorGtk::Delegate,
                         public MessageLoopForUI::Observer {
 public:
  explicit DownloadShelfGtk(Browser* browser, gfx::NativeView view);

  ~DownloadShelfGtk();

  
  virtual void AddDownload(BaseDownloadItemModel* download_model);
  virtual bool IsShowing() const;
  virtual bool IsClosing() const;
  virtual void Show();
  virtual void Close();
  virtual Browser* browser() const;

  
  virtual void Closed();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  int GetHeight() const;

  
  virtual void WillProcessEvent(GdkEvent* event);
  virtual void DidProcessEvent(GdkEvent* event);

 private:
  
  void RemoveDownloadItem(DownloadItemGtk* download_item);

  
  GtkWidget* GetHBox() const;

  
  
  
  void MaybeShowMoreDownloadItems();

  
  
  void AutoCloseIfPossible();

  
  
  void CancelAutoClose();

  
  
  void ItemOpened();

  
  void SetCloseOnMouseOut(bool close);

  
  
  bool IsCursorInShelfZone(const gfx::Point& cursor_screen_coords);

  
  void MouseLeftShelf();
  void MouseEnteredShelf();

  CHROMEGTK_CALLBACK_0(DownloadShelfGtk, void, OnButtonClick);

  
  Browser* browser_;

  
  scoped_ptr<SlideAnimatorGtk> slide_widget_;

  
  OwnedWidgetGtk items_hbox_;

  
  
  OwnedWidgetGtk shelf_;

  
  GtkWidget* top_border_;

  
  GtkWidget* padding_bg_;

  
  GtkWidget* link_button_;

  
  scoped_ptr<CustomDrawButton> close_button_;

  
  bool is_showing_;

  
  std::vector<DownloadItemGtk*> download_items_;

  
  GtkThemeService* theme_service_;

  NotificationRegistrar registrar_;

  
  bool close_on_mouse_out_;

  
  
  bool mouse_in_shelf_;

  ScopedRunnableMethodFactory<DownloadShelfGtk> auto_close_factory_;

  friend class DownloadItemGtk;
};

#endif  
