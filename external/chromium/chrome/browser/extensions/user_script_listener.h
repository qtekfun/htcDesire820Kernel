// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_USER_SCRIPT_LISTENER_H_
#define CHROME_BROWSER_EXTENSIONS_USER_SCRIPT_LISTENER_H_
#pragma once

#include <list>

#include "base/memory/ref_counted.h"
#include "content/browser/renderer_host/resource_queue.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

namespace net {
class URLRequest;
}  

class Extension;
class URLPattern;
struct GlobalRequestID;

class UserScriptListener
    : public base::RefCountedThreadSafe<UserScriptListener>,
      public ResourceQueueDelegate,
      public NotificationObserver {
 public:
  UserScriptListener();

  
  virtual void Initialize(ResourceQueue* resource_queue);
  virtual bool ShouldDelayRequest(
      net::URLRequest* request,
      const ResourceDispatcherHostRequestInfo& request_info,
      const GlobalRequestID& request_id);
  virtual void WillShutdownResourceQueue();

 private:
  friend class base::RefCountedThreadSafe<UserScriptListener>;

  typedef std::list<URLPattern> URLPatterns;

  ~UserScriptListener();

  
  void StartDelayedRequests();

  
  
  void AppendNewURLPatterns(const URLPatterns& new_patterns);

  
  
  void ReplaceURLPatterns(const URLPatterns& patterns);

  
  void Cleanup();

  ResourceQueue* resource_queue_;

  
  
  typedef std::list<GlobalRequestID> DelayedRequests;
  DelayedRequests delayed_request_ids_;

  
  

  
  
  bool user_scripts_ready_;

  
  URLPatterns url_patterns_;

  

  
  
  void CollectURLPatterns(const Extension* extension, URLPatterns* patterns);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(UserScriptListener);
};

#endif  
