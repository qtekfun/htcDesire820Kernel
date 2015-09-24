// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_SERVICE_H_
#define CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_SERVICE_H_

#include <map>
#include <string>

#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/sequenced_task_runner_helpers.h"
#include "chrome/browser/safe_browsing/safe_browsing_util.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class PrefChangeRegistrar;
class PrefService;
class SafeBrowsingDatabaseManager;
class SafeBrowsingPingManager;
class SafeBrowsingProtocolManager;
class SafeBrowsingServiceFactory;
class SafeBrowsingUIManager;
class SafeBrowsingURLRequestContextGetter;

namespace base {
class Thread;
}

namespace net {
class URLRequestContext;
class URLRequestContextGetter;
}

namespace safe_browsing {
class ClientSideDetectionService;
class DownloadProtectionService;
}

class SafeBrowsingService
    : public base::RefCountedThreadSafe<
          SafeBrowsingService,
          content::BrowserThread::DeleteOnUIThread>,
      public content::NotificationObserver {
 public:
  
  
  static void RegisterFactory(SafeBrowsingServiceFactory* factory) {
    factory_ = factory;
  }

  static base::FilePath GetCookieFilePathForTesting();

  static base::FilePath GetBaseFilename();

  
  static SafeBrowsingService* CreateSafeBrowsingService();

  
  void Initialize();

  
  void ShutDown();

  
  
  bool DownloadBinHashNeeded() const;

  bool enabled() const { return enabled_; }

  safe_browsing::ClientSideDetectionService*
      safe_browsing_detection_service() const {
    DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));
    return csd_service_.get();
  }

  
  
  safe_browsing::DownloadProtectionService*
      download_protection_service() const {
    DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));
    return download_service_.get();
  }

  net::URLRequestContextGetter* url_request_context();

  const scoped_refptr<SafeBrowsingUIManager>& ui_manager() const;

  const scoped_refptr<SafeBrowsingDatabaseManager>& database_manager() const;

  SafeBrowsingProtocolManager* protocol_manager() const;

  SafeBrowsingPingManager* ping_manager() const;

 protected:
  
  SafeBrowsingService();

  virtual ~SafeBrowsingService();

  virtual SafeBrowsingDatabaseManager* CreateDatabaseManager();

  virtual SafeBrowsingUIManager* CreateUIManager();

 private:
  friend class SafeBrowsingServiceFactoryImpl;
  friend struct content::BrowserThread::DeleteOnThread<
      content::BrowserThread::UI>;
  friend class base::DeleteHelper<SafeBrowsingService>;
  friend class SafeBrowsingServerTest;
  friend class SafeBrowsingServiceTest;
  friend class SafeBrowsingURLRequestContextGetter;

  void InitURLRequestContextOnIOThread(
      net::URLRequestContextGetter* system_url_request_context_getter);

  void DestroyURLRequestContextOnIOThread();

  
  
  void StartOnIOThread(
      net::URLRequestContextGetter* url_request_context_getter);

  
  
  
  
  void StopOnIOThread(bool shutdown);

  
  
  
  void Start();

  
  
  
  void Stop(bool shutdown);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void AddPrefService(PrefService* pref_service);

  
  void RemovePrefService(PrefService* pref_service);

  
  
  void RefreshState();

  
  
  
  static SafeBrowsingServiceFactory* factory_;

  
  
  scoped_refptr<net::URLRequestContextGetter>
      url_request_context_getter_;

  
  scoped_ptr<net::URLRequestContext> url_request_context_;

  
  SafeBrowsingProtocolManager* protocol_manager_;

  
  SafeBrowsingPingManager* ping_manager_;

  
  
  bool enabled_;

  
  
  
  
  std::map<PrefService*, PrefChangeRegistrar*> prefs_map_;

  
  content::NotificationRegistrar prefs_registrar_;

  
  
  
  scoped_ptr<safe_browsing::ClientSideDetectionService> csd_service_;

  
  
  
  scoped_ptr<safe_browsing::DownloadProtectionService> download_service_;

  
  
  scoped_refptr<SafeBrowsingUIManager> ui_manager_;

  
  
  scoped_refptr<SafeBrowsingDatabaseManager> database_manager_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingService);
};

class SafeBrowsingServiceFactory {
 public:
  SafeBrowsingServiceFactory() { }
  virtual ~SafeBrowsingServiceFactory() { }
  virtual SafeBrowsingService* CreateSafeBrowsingService() = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingServiceFactory);
};

#endif  
