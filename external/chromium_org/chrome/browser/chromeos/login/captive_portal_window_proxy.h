// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_CAPTIVE_PORTAL_WINDOW_PROXY_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_CAPTIVE_PORTAL_WINDOW_PROXY_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/views/widget/widget_observer.h"

namespace content {
class WebContents;
}

namespace views {
class Widget;
}

namespace chromeos {

class CaptivePortalView;

class CaptivePortalWindowProxyDelegate {
 public:
  
  virtual void OnPortalDetected() = 0;

 protected:
  virtual ~CaptivePortalWindowProxyDelegate() {}
};

class CaptivePortalWindowProxy : public views::WidgetObserver {
 public:
  typedef CaptivePortalWindowProxyDelegate Delegate;

  CaptivePortalWindowProxy(Delegate* delegate,
                           content::WebContents* web_contents);
  virtual ~CaptivePortalWindowProxy();

  
  
  
  
  
  void ShowIfRedirected();

  
  void Show();

  
  void Close();

  
  
  void OnRedirected();

  
  
  void OnOriginalURLLoaded();

  
  virtual void OnWidgetClosing(views::Widget* widget) OVERRIDE;
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;
  virtual void OnWidgetDestroyed(views::Widget* widget) OVERRIDE;

 private:
  friend class CaptivePortalWindowTest;

  
  
  
  
  
  
  
  
  
  
  enum State {
    STATE_IDLE = 0,
    STATE_WAITING_FOR_REDIRECTION,
    STATE_DISPLAYED,
    STATE_UNKNOWN
  };

  
  
  void InitCaptivePortalView();

  
  State GetState() const;

  
  
  void DetachFromWidget(views::Widget* widget);

  
  Delegate* delegate_;
  
  views::Widget* widget_;
  scoped_ptr<CaptivePortalView> captive_portal_view_;

  
  content::WebContents* web_contents_;

  DISALLOW_COPY_AND_ASSIGN(CaptivePortalWindowProxy);
};

}  

#endif  
