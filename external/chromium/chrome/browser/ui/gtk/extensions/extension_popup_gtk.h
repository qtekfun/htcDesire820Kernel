// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_EXTENSIONS_EXTENSION_POPUP_GTK_H_
#define CHROME_BROWSER_UI_GTK_EXTENSIONS_EXTENSION_POPUP_GTK_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "chrome/browser/ui/gtk/extensions/extension_view_gtk.h"
#include "chrome/browser/ui/gtk/info_bubble_gtk.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/gfx/rect.h"

class Browser;
class ExtensionHost;
class GURL;

class ExtensionPopupGtk : public NotificationObserver,
                          public InfoBubbleGtkDelegate,
                          public ExtensionViewGtk::Container {
 public:
  ExtensionPopupGtk(Browser* browser,
                    ExtensionHost* host,
                    GtkWidget* anchor,
                    bool inspect);
  virtual ~ExtensionPopupGtk();

  static void Show(const GURL& url,
                   Browser* browser,
                   GtkWidget* anchor,
                   bool inspect);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void InfoBubbleClosing(InfoBubbleGtk* bubble,
                                 bool closed_by_escape);

  
  virtual void OnExtensionPreferredSizeChanged(ExtensionViewGtk* view,
                                               const gfx::Size& new_size);

  
  
  
  bool DestroyPopup();

  
  static ExtensionPopupGtk* get_current_extension_popup() {
    return current_extension_popup_;
  }

  bool being_inspected() const {
    return being_inspected_;
  }

  
  static const int kMinWidth;
  static const int kMinHeight;
  static const int kMaxWidth;
  static const int kMaxHeight;

 private:
  
  void ShowPopup();

  Browser* browser_;

  InfoBubbleGtk* bubble_;

  
  scoped_ptr<ExtensionHost> host_;

  
  GtkWidget* anchor_;

  NotificationRegistrar registrar_;

  static ExtensionPopupGtk* current_extension_popup_;

  
  bool being_inspected_;

  ScopedRunnableMethodFactory<ExtensionPopupGtk> method_factory_;

  
  gfx::Rect GetViewBounds();

  friend class BrowserActionTestUtil;

  DISALLOW_COPY_AND_ASSIGN(ExtensionPopupGtk);
};

#endif  
