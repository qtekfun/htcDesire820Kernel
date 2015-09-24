// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_PUBLIC_BROWSER_NOTIFICATION_SERVICE_H_
#define CONTENT_PUBLIC_BROWSER_NOTIFICATION_SERVICE_H_

#include "content/common/content_export.h"
#include "content/public/browser/notification_details.h"
#include "content/public/browser/notification_source.h"

namespace content {

class CONTENT_EXPORT NotificationService {
 public:
  
  
  static NotificationService* current();

  static NotificationService* Create();

  virtual ~NotificationService() {}

  
  
  
  
  
  
  
  
  virtual void Notify(int type,
                      const NotificationSource& source,
                      const NotificationDetails& details) = 0;

  
  
  static Source<void> AllSources() { return Source<void>(NULL); }

  
  
  
  
  
  
  
  
  
  
  static Source<void> AllBrowserContextsAndSources() {
    return Source<void>(NULL);
  }

  
  
  static Details<void> NoDetails() { return Details<void>(NULL); }
};

}  

#endif  
