// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_CHROME_NOTIFICATION_OBSERVER_H_
#define CHROME_BROWSER_EXTENSIONS_CHROME_NOTIFICATION_OBSERVER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Browser;

namespace extensions {

class ChromeNotificationObserver : public content::NotificationObserver {
 public:
  ChromeNotificationObserver();
  virtual ~ChromeNotificationObserver();

  void OnBrowserWindowReady(Browser* browser);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ChromeNotificationObserver);
};

}  

#endif  
