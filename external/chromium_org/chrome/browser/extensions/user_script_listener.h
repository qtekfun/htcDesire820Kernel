// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_USER_SCRIPT_LISTENER_H_
#define CHROME_BROWSER_EXTENSIONS_USER_SCRIPT_LISTENER_H_

#include <deque>
#include <list>
#include <map>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "webkit/common/resource_type.h"

class GURL;
class URLPattern;

namespace content {
class ResourceThrottle;
}

namespace extensions {
class Extension;

class UserScriptListener
    : public base::RefCountedThreadSafe<
          UserScriptListener,
          content::BrowserThread::DeleteOnUIThread>,
      public content::NotificationObserver {
 public:
  UserScriptListener();

  
  
  content::ResourceThrottle* CreateResourceThrottle(
      const GURL& url,
      ResourceType::Type resource_type);

 private:
  friend struct content::BrowserThread::DeleteOnThread<
      content::BrowserThread::UI>;
  friend class base::DeleteHelper<UserScriptListener>;

  typedef std::list<URLPattern> URLPatterns;

  virtual ~UserScriptListener();

  bool ShouldDelayRequest(const GURL& url, ResourceType::Type resource_type);
  void StartDelayedRequests();

  
  
  void CheckIfAllUserScriptsReady();

  
  void UserScriptsReady(void* profile_id);

  
  void ProfileDestroyed(void* profile_id);

  
  
  void AppendNewURLPatterns(void* profile_id, const URLPatterns& new_patterns);

  
  
  void ReplaceURLPatterns(void* profile_id, const URLPatterns& patterns);

  
  bool user_scripts_ready_;

  
  class Throttle;
  typedef base::WeakPtr<Throttle> WeakThrottle;
  typedef std::deque<WeakThrottle> WeakThrottleList;
  WeakThrottleList throttles_;

  
  struct ProfileData;
  typedef std::map<void*, ProfileData> ProfileDataMap;
  ProfileDataMap profile_data_;

  

  
  
  void CollectURLPatterns(const Extension* extension,
                          URLPatterns* patterns);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(UserScriptListener);
};

}  

#endif  
