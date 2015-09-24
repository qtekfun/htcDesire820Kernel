// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_POPUP_H_
#define CHROME_BROWSER_UI_VIEWS_EXTENSIONS_EXTENSION_POPUP_H_

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "chrome/browser/ui/views/extensions/extension_view_views.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/focus/widget_focus_manager.h"
#include "url/gurl.h"

#if defined(USE_AURA)
#include "ui/aura/client/activation_change_observer.h"
#endif

class Browser;
namespace views {
class Widget;
}

namespace content {
class DevToolsAgentHost;
}

namespace extensions {
class ExtensionViewHost;
}

class ExtensionPopup : public views::BubbleDelegateView,
#if defined(USE_AURA)
                       public aura::client::ActivationChangeObserver,
#endif
                       public ExtensionViewViews::Container,
                       public content::NotificationObserver,
                       public TabStripModelObserver {
 public:
  enum ShowAction {
    SHOW,
    SHOW_AND_INSPECT
  };

  virtual ~ExtensionPopup();

  
  
  
  
  
  
  
  
  
  static ExtensionPopup* ShowPopup(const GURL& url,
                                   Browser* browser,
                                   views::View* anchor_view,
                                   views::BubbleBorder::Arrow arrow,
                                   ShowAction show_action);

  extensions::ExtensionViewHost* host() const { return host_.get(); }

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnExtensionSizeChanged(ExtensionViewViews* view) OVERRIDE;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;
  virtual void OnWidgetActivationChanged(views::Widget* widget,
                                         bool active) OVERRIDE;

#if defined(USE_AURA)
  
  virtual void OnWindowActivated(aura::Window* gained_active,
                                 aura::Window* lost_active) OVERRIDE;
#endif

  
  virtual void ActiveTabChanged(content::WebContents* old_contents,
                                content::WebContents* new_contents,
                                int index,
                                int reason) OVERRIDE;

  
  static const int kMinWidth;
  static const int kMinHeight;
  static const int kMaxWidth;
  static const int kMaxHeight;

 private:
  ExtensionPopup(extensions::ExtensionViewHost* host,
                 views::View* anchor_view,
                 views::BubbleBorder::Arrow arrow,
                 ShowAction show_action);

  
  void ShowBubble();

  void OnDevToolsStateChanged(content::DevToolsAgentHost*, bool attached);

  
  scoped_ptr<extensions::ExtensionViewHost> host_;

  
  
  bool inspect_with_devtools_;

  content::NotificationRegistrar registrar_;

  base::Callback<void(content::DevToolsAgentHost*, bool)> devtools_callback_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionPopup);
};

#endif  
