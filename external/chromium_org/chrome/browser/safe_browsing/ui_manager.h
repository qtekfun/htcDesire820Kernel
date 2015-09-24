// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_UI_MANAGER_H_
#define CHROME_BROWSER_SAFE_BROWSING_UI_MANAGER_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "chrome/browser/safe_browsing/safe_browsing_util.h"
#include "content/public/browser/notification_observer.h"
#include "url/gurl.h"

class SafeBrowsingService;

namespace base {
class Thread;
}

class SafeBrowsingUIManager
    : public base::RefCountedThreadSafe<SafeBrowsingUIManager> {
 public:
  
  
  typedef base::Callback<void(bool )> UrlCheckCallback;

  
  
  struct UnsafeResource {
    UnsafeResource();
    ~UnsafeResource();

    GURL url;
    GURL original_url;
    std::vector<GURL> redirect_urls;
    bool is_subresource;
    SBThreatType threat_type;
    UrlCheckCallback callback;
    int render_process_host_id;
    int render_view_id;
  };

  
  
  class Observer {
   public:
    
    
    
    
    
    virtual void OnSafeBrowsingMatch(const UnsafeResource& resource) = 0;

    
    
    
    
    virtual void OnSafeBrowsingHit(const UnsafeResource& resource) = 0;

   protected:
    Observer() {}
    virtual ~Observer() {}

   private:
    DISALLOW_COPY_AND_ASSIGN(Observer);
  };

  explicit SafeBrowsingUIManager(
      const scoped_refptr<SafeBrowsingService>& service);

  
  
  
  void StopOnIOThread(bool shutdown);

  
  
  virtual bool CanReportStats() const;

  
  
  
  
  
  void DisplayBlockingPage(const GURL& url,
                           const GURL& original_url,
                           const std::vector<GURL>& redirect_urls,
                           bool is_subresource,
                           SBThreatType threat_type,
                           const UrlCheckCallback& callback,
                           int render_process_host_id,
                           int render_view_id);

  
  virtual void DoDisplayBlockingPage(const UnsafeResource& resource);

  
  
  bool IsWhitelisted(const UnsafeResource& resource);

  
  void OnBlockingPageDone(const std::vector<UnsafeResource>& resources,
                          bool proceed);

  
  
  
  
  void LogPauseDelay(base::TimeDelta time);

  
  
  virtual void SendSerializedMalwareDetails(const std::string& serialized);

  
  
  
  virtual void ReportSafeBrowsingHit(const GURL& malicious_url,
                                     const GURL& page_url,
                                     const GURL& referrer_url,
                                     bool is_subresource,
                                     SBThreatType threat_type,
                                     const std::string& post_data);

  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* remove);

 protected:
  virtual ~SafeBrowsingUIManager();

 private:
  friend class base::RefCountedThreadSafe<SafeBrowsingUIManager>;

  
  struct WhiteListedEntry;

  
  void ReportSafeBrowsingHitOnIOThread(const GURL& malicious_url,
                                       const GURL& page_url,
                                       const GURL& referrer_url,
                                       bool is_subresource,
                                       SBThreatType threat_type,
                                       const std::string& post_data);

  
  void UpdateWhitelist(const UnsafeResource& resource);

  
  scoped_refptr<SafeBrowsingService> sb_service_;

  
  std::vector<WhiteListedEntry> white_listed_entries_;

  ObserverList<Observer> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingUIManager);
};

#endif  
