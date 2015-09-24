// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SESSIONS2_NOTIFICATION_SERVICE_SESSIONS_ROUTER_H_
#define CHROME_BROWSER_SYNC_SESSIONS2_NOTIFICATION_SERVICE_SESSIONS_ROUTER_H_

#include "base/memory/weak_ptr.h"
#include "chrome/browser/sync/sessions2/sessions_sync_manager.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Profile;

namespace content {
class WebContents;
}

namespace browser_sync {

class NotificationServiceSessionsRouter
    : public LocalSessionEventRouter,
      public content::NotificationObserver {
 public:
  NotificationServiceSessionsRouter(
      Profile* profile,
      const syncer::SyncableService::StartSyncFlare& flare);
  virtual ~NotificationServiceSessionsRouter();

  
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void StartRoutingTo(LocalSessionEventHandler* handler) OVERRIDE;
  virtual void Stop() OVERRIDE;

 private:
  
  
  
  
  
  void OnNavigationBlocked(content::WebContents* web_contents);

  LocalSessionEventHandler* handler_;
  content::NotificationRegistrar registrar_;
  Profile* const profile_;
  syncer::SyncableService::StartSyncFlare flare_;
  base::WeakPtrFactory<NotificationServiceSessionsRouter> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(NotificationServiceSessionsRouter);
};

}  

#endif  
