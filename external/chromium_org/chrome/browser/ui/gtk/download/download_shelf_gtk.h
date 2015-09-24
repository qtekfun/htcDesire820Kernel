// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_DOWNLOAD_DOWNLOAD_SHELF_GTK_H_
#define CHROME_BROWSER_UI_GTK_DOWNLOAD_DOWNLOAD_SHELF_GTK_H_

#include <gtk/gtk.h>

#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "chrome/browser/download/download_shelf.h"
#include "chrome/browser/ui/gtk/slide_animator_gtk.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/gfx/native_widget_types.h"

class Browser;
class CustomDrawButton;
class DownloadItemGtk;
class GtkThemeService;

namespace content {
class PageNavigator;
}

namespace gfx {
class Point;
}

class DownloadShelfGtk : public DownloadShelf,
                         public content::NotificationObserver,
                         public SlideAnimatorGtk::Delegate,
                         public base::MessageLoopForUI::Observer {
 public:
  DownloadShelfGtk(Browser* browser, gfx::NativeView view);

  virtual ~DownloadShelfGtk();

  
  content::PageNavigator* GetNavigator();

  
  virtual bool IsShowing() const OVERRIDE;
  virtual bool IsClosing() const OVERRIDE;
  virtual Browser* browser() const OVERRIDE;

  
  virtual void Closed() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  int GetHeight() const;

  
  virtual void WillProcessEvent(GdkEvent* event) OVERRIDE;
  virtual void DidProcessEvent(GdkEvent* event) OVERRIDE;

 protected:
  
  virtual void DoAddDownload(content::DownloadItem* download) OVERRIDE;
  virtual void DoShow() OVERRIDE;
  virtual void DoClose(CloseReason reason) OVERRIDE;

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

  
  ui::OwnedWidgetGtk items_hbox_;

  
  
  ui::OwnedWidgetGtk shelf_;

  
  GtkWidget* top_border_;

  
  GtkWidget* padding_bg_;

  
  GtkWidget* link_button_;

  
  scoped_ptr<CustomDrawButton> close_button_;

  
  bool is_showing_;

  
  std::vector<DownloadItemGtk*> download_items_;

  
  GtkThemeService* theme_service_;

  content::NotificationRegistrar registrar_;

  
  bool close_on_mouse_out_;

  
  
  bool mouse_in_shelf_;

  base::WeakPtrFactory<DownloadShelfGtk> weak_factory_;

  friend class DownloadItemGtk;
};

#endif  
