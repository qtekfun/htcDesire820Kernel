// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_INSTALLED_BUBBLE_H_
#define CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_INSTALLED_BUBBLE_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "chrome/browser/ui/views/bubble/bubble.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "third_party/skia/include/core/SkBitmap.h"

class Browser;
class Extension;
class InstalledBubbleContent;
class SkBitmap;

class ExtensionInstalledBubble
    : public BubbleDelegate,
      public NotificationObserver,
      public base::RefCountedThreadSafe<ExtensionInstalledBubble> {
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
  friend class base::RefCountedThreadSafe<ExtensionInstalledBubble>;

  
  ExtensionInstalledBubble(
      const Extension* extension, Browser *browser, const SkBitmap& icon);

  virtual ~ExtensionInstalledBubble();

  
  void ShowInternal();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void BubbleClosing(Bubble* bubble, bool closed_by_escape);
  virtual bool CloseOnEscape();
  virtual bool FadeInOnShow();

  const Extension* extension_;
  Browser* browser_;
  SkBitmap icon_;
  NotificationRegistrar registrar_;
  InstalledBubbleContent* bubble_content_;
  BubbleType type_;

  
  int animation_wait_retries_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionInstalledBubble);
};

#endif  
