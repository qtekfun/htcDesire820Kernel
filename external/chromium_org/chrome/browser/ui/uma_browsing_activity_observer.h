// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_UMA_BROWSING_ACTIVITY_OBSERVER_H_
#define CHROME_BROWSER_UI_UMA_BROWSING_ACTIVITY_OBSERVER_H_

#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/notification_observer.h"

namespace chrome {

class UMABrowsingActivityObserver : public content::NotificationObserver {
 public:
  static void Init();

 private:
  UMABrowsingActivityObserver();
  virtual ~UMABrowsingActivityObserver();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void LogRenderProcessHostCount() const;

  
  
  
  
  void LogBrowserTabCount() const;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(UMABrowsingActivityObserver);
};

}  

#endif  
