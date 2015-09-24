// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_EXTENSIONS_EXTENSION_INSTALLED_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_EXTENSIONS_EXTENSION_INSTALLED_BUBBLE_GTK_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/gtk/custom_button.h"
#include "chrome/browser/ui/gtk/info_bubble_gtk.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "third_party/skia/include/core/SkBitmap.h"

class Browser;
class BrowserWindowGtk;
class Extension;
class SkBitmap;

class ExtensionInstalledBubbleGtk
    : public InfoBubbleGtkDelegate,
      public NotificationObserver,
      public base::RefCountedThreadSafe<ExtensionInstalledBubbleGtk> {
 public:
  
  enum BubbleType {
    OMNIBOX_KEYWORD,
    BROWSER_ACTION,
    PAGE_ACTION,
    GENERIC
  };

  
  
  
  
  static void Show(
      const Extension* extension, Browser *browser, const SkBitmap& icon);

 private:
  friend class base::RefCountedThreadSafe<ExtensionInstalledBubbleGtk>;

  
  ExtensionInstalledBubbleGtk(const Extension* extension, Browser *browser,
                              const SkBitmap& icon);

  virtual ~ExtensionInstalledBubbleGtk();

  
  void ShowInternal();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void InfoBubbleClosing(InfoBubbleGtk* info_bubble,
                                 bool closed_by_escape);

  
  void Close();

  static void OnButtonClick(GtkWidget* button,
                            ExtensionInstalledBubbleGtk* toolbar);

  const Extension* extension_;
  Browser *browser_;
  SkBitmap icon_;
  NotificationRegistrar registrar_;
  BubbleType type_;

  
  
  int animation_wait_retries_;

  
  scoped_ptr<CustomDrawButton> close_button_;

  InfoBubbleGtk* info_bubble_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionInstalledBubbleGtk);
};

#endif  
