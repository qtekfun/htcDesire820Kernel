// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_NOTIFICATION_SERVICE_IMPL_H_
#define CONTENT_PUBLIC_BROWSER_NOTIFICATION_SERVICE_IMPL_H_

#include <map>

#include "base/observer_list.h"
#include "content/common/content_export.h"
#include "content/public/browser/notification_service.h"

namespace content {

class NotificationObserver;
class NotificationRegistrar;

class CONTENT_EXPORT NotificationServiceImpl : public NotificationService {
 public:
  static NotificationServiceImpl* current();

  
  
  NotificationServiceImpl();
  virtual ~NotificationServiceImpl();

  
  virtual void Notify(int type,
                      const NotificationSource& source,
                      const NotificationDetails& details) OVERRIDE;

 private:
  friend class NotificationRegistrar;

  typedef ObserverList<NotificationObserver> NotificationObserverList;
  typedef std::map<uintptr_t, NotificationObserverList*> NotificationSourceMap;
  typedef std::map<int, NotificationSourceMap> NotificationObserverMap;
  typedef std::map<int, int> NotificationObserverCount;

  
  
  static bool HasKey(const NotificationSourceMap& map,
                     const NotificationSource& source);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void AddObserver(NotificationObserver* observer,
                   int type,
                   const NotificationSource& source);

  
  
  
  
  
  
  void RemoveObserver(NotificationObserver* observer,
                      int type,
                      const NotificationSource& source);

  
  
  
  NotificationObserverMap observers_;

#ifndef NDEBUG
  
  
  NotificationObserverCount observer_counts_;
#endif

  DISALLOW_COPY_AND_ASSIGN(NotificationServiceImpl);
};

}  

#endif  
