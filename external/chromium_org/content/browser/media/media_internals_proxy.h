// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_MEDIA_MEDIA_INTERNALS_PROXY_H_
#define CONTENT_BROWSER_MEDIA_MEDIA_INTERNALS_PROXY_H_

#include "base/memory/ref_counted.h"
#include "base/sequenced_task_runner_helpers.h"
#include "content/browser/media/media_internals.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "net/base/net_log.h"

namespace base {
class ListValue;
class Value;
}

namespace content {
class MediaInternalsMessageHandler;

class MediaInternalsProxy
    : public base::RefCountedThreadSafe<
          MediaInternalsProxy,
          BrowserThread::DeleteOnUIThread>,
      public net::NetLog::ThreadSafeObserver,
      public NotificationObserver {
 public:
  MediaInternalsProxy();

  
  virtual void Observe(int type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  void Attach(MediaInternalsMessageHandler* handler);

  
  void Detach();

  
  void GetEverything();

  
  void OnUpdate(const base::string16& update);

  
  virtual void OnAddEntry(const net::NetLog::Entry& entry) OVERRIDE;

 private:
  friend struct BrowserThread::DeleteOnThread<BrowserThread::UI>;
  friend class base::DeleteHelper<MediaInternalsProxy>;
  virtual ~MediaInternalsProxy();

  
  base::Value* GetConstants();

  void ObserveMediaInternalsOnIOThread();
  void StopObservingMediaInternalsOnIOThread();
  void GetEverythingOnIOThread();
  void UpdateUIOnUIThread(const base::string16& update);

  
  void AddNetEventOnUIThread(base::Value* entry);

  
  void SendNetEventsOnUIThread();

  
  void CallJavaScriptFunctionOnUIThread(const std::string& function,
                                        base::Value* args);

  MediaInternalsMessageHandler* handler_;
  scoped_ptr<base::ListValue> pending_net_updates_;
  NotificationRegistrar registrar_;
  MediaInternals::UpdateCallback update_callback_;

  DISALLOW_COPY_AND_ASSIGN(MediaInternalsProxy);
};

}  

#endif  
