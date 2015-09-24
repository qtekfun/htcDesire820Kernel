// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_POPUP_H_
#define CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_POPUP_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "chrome/browser/extensions/extension_host.h"
#include "chrome/browser/ui/views/browser_bubble.h"
#include "chrome/browser/ui/views/bubble/bubble_border.h"
#include "chrome/browser/ui/views/extensions/extension_view.h"
#include "content/common/notification_observer.h"
#include "googleurl/src/gurl.h"


class Browser;
class ExtensionHost;
class Profile;

namespace views {
class Widget;
}

class ExtensionPopup : public BrowserBubble,
                       public BrowserBubble::Delegate,
                       public ExtensionView::Container,
                       public NotificationObserver,
                       public base::RefCounted<ExtensionPopup> {
 public:
  
  class Observer {
   public:
    
    
    
    virtual void ExtensionPopupIsClosing(ExtensionPopup* popup) {}
  };

  virtual ~ExtensionPopup();

  
  
  
  
  
  
  
  
  
  
  
  
  
  static ExtensionPopup* Show(const GURL& url, Browser* browser,
                              const gfx::Rect& relative_to,
                              BubbleBorder::ArrowLocation arrow_location,
                              bool inspect_with_devtools,
                              Observer* observer);

  
  void Close();

  
  
  
  void set_close_on_lost_focus(bool close_on_lost_focus) {
    close_on_lost_focus_ = close_on_lost_focus;
  }

  ExtensionHost* host() const { return extension_host_.get(); }

  
  virtual void Show(bool activate);

  
  virtual void BubbleBrowserWindowMoved(BrowserBubble* bubble);
  virtual void BubbleBrowserWindowClosing(BrowserBubble* bubble);
  virtual void BubbleGotFocus(BrowserBubble* bubble);
  virtual void BubbleLostFocus(BrowserBubble* bubble,
                               bool lost_focus_to_child);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void OnExtensionMouseMove(ExtensionView* view) { }
  virtual void OnExtensionMouseLeave(ExtensionView* view) { }
  virtual void OnExtensionPreferredSizeChanged(ExtensionView* view);

  
  static const int kMinWidth;
  static const int kMinHeight;
  static const int kMaxWidth;
  static const int kMaxHeight;

 private:
  ExtensionPopup(ExtensionHost* host,
                 views::Widget* frame,
                 const gfx::Rect& relative_to,
                 BubbleBorder::ArrowLocation arrow_location,
                 bool inspect_with_devtools,
                 Observer* observer);

  
  gfx::Rect relative_to_;

  
  scoped_ptr<ExtensionHost> extension_host_;

  
  
  bool inspect_with_devtools_;

  
  bool close_on_lost_focus_;

  
  bool closing_;

  NotificationRegistrar registrar_;

  
  Observer* observer_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionPopup);
};

#endif  
