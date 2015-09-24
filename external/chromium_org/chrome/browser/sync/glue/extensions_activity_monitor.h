// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_EXTENSIONS_ACTIVITY_MONITOR_H_
#define CHROME_BROWSER_SYNC_GLUE_EXTENSIONS_ACTIVITY_MONITOR_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace syncer {
class ExtensionsActivity;
}

namespace browser_sync {

class ExtensionsActivityMonitor : public content::NotificationObserver {
 public:
  ExtensionsActivityMonitor();
  virtual ~ExtensionsActivityMonitor();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  const scoped_refptr<syncer::ExtensionsActivity>& GetExtensionsActivity();

 private:
  scoped_refptr<syncer::ExtensionsActivity> extensions_activity_;

  
  content::NotificationRegistrar registrar_;
};

}  

#endif  