// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_SAFE_BROWSING_RESOURCE_THROTTLE_H_
#define CHROME_BROWSER_RENDERER_HOST_SAFE_BROWSING_RESOURCE_THROTTLE_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/safe_browsing/database_manager.h"
#include "chrome/browser/safe_browsing/ui_manager.h"
#include "content/public/browser/resource_throttle.h"

class ResourceDispatcherHost;

namespace net {
class URLRequest;
}

class SafeBrowsingResourceThrottle
    : public content::ResourceThrottle,
      public SafeBrowsingDatabaseManager::Client,
      public base::SupportsWeakPtr<SafeBrowsingResourceThrottle> {
 public:
  SafeBrowsingResourceThrottle(const net::URLRequest* request,
                               bool is_subresource,
                               SafeBrowsingService* safe_browsing);

  
  virtual void WillStartRequest(bool* defer) OVERRIDE;
  virtual void WillRedirectRequest(const GURL& new_url, bool* defer) OVERRIDE;
  virtual const char* GetNameForLogging() const OVERRIDE;

  
  virtual void OnCheckBrowseUrlResult(
      const GURL& url, SBThreatType result) OVERRIDE;

 private:
  
  enum State {
    STATE_NONE,
    STATE_CHECKING_URL,
    STATE_DISPLAYING_BLOCKING_PAGE,
  };

  
  enum DeferState {
    DEFERRED_NONE,
    DEFERRED_START,
    DEFERRED_REDIRECT,
  };

  virtual ~SafeBrowsingResourceThrottle();

  
  void OnBlockingPageComplete(bool proceed);

  
  
  
  bool CheckUrl(const GURL& url);

  
  
  void OnCheckUrlTimeout();

  
  void StartDisplayingBlockingPage(const GURL& url, SBThreatType threat_type);

  
  
  void ResumeRequest();

  State state_;
  DeferState defer_state_;

  
  
  SBThreatType threat_type_;

  
  base::TimeTicks url_check_start_time_;

  
  base::OneShotTimer<SafeBrowsingResourceThrottle> timer_;

  
  std::vector<GURL> redirect_urls_;

  GURL url_being_checked_;

  scoped_refptr<SafeBrowsingDatabaseManager> database_manager_;
  scoped_refptr<SafeBrowsingUIManager> ui_manager_;
  const net::URLRequest* request_;
  bool is_subresource_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingResourceThrottle);
};


#endif  
