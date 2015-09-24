// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_VISITEDLINK_BROWSER_VISITEDLINK_EVENT_LISTENER_H_
#define COMPONENTS_VISITEDLINK_BROWSER_VISITEDLINK_EVENT_LISTENER_H_

#include <map>

#include "base/memory/linked_ptr.h"
#include "base/timer/timer.h"
#include "components/visitedlink/browser/visitedlink_master.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace base {
class SharedMemory;
}

namespace content {
class BrowserContext;
}

namespace visitedlink {

class VisitedLinkUpdater;

class VisitedLinkEventListener : public VisitedLinkMaster::Listener,
                                 public content::NotificationObserver {
 public:
  VisitedLinkEventListener(VisitedLinkMaster* master,
                           content::BrowserContext* browser_context);
  virtual ~VisitedLinkEventListener();

  virtual void NewTable(base::SharedMemory* table_memory) OVERRIDE;
  virtual void Add(VisitedLinkMaster::Fingerprint fingerprint) OVERRIDE;
  virtual void Reset() OVERRIDE;

 private:
  void CommitVisitedLinks();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  base::OneShotTimer<VisitedLinkEventListener> coalesce_timer_;
  VisitedLinkCommon::Fingerprints pending_visited_links_;

  content::NotificationRegistrar registrar_;

  
  typedef std::map<int, linked_ptr<VisitedLinkUpdater> > Updaters;
  Updaters updaters_;

  VisitedLinkMaster* master_;

  
  
  content::BrowserContext* browser_context_;

  DISALLOW_COPY_AND_ASSIGN(VisitedLinkEventListener);
};

}  

#endif  
