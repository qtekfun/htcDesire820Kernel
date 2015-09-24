// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_CLIENT_SIDE_DETECTION_HOST_H_
#define CHROME_BROWSER_SAFE_BROWSING_CLIENT_SIDE_DETECTION_HOST_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/safe_browsing/browser_feature_extractor.h"
#include "chrome/browser/safe_browsing/database_manager.h"
#include "chrome/browser/safe_browsing/ui_manager.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/resource_request_details.h"
#include "content/public/browser/web_contents_observer.h"
#include "url/gurl.h"

namespace safe_browsing {
class ClientPhishingRequest;
class ClientSideDetectionService;

class ClientSideDetectionHost : public content::WebContentsObserver,
                                public content::NotificationObserver,
                                public SafeBrowsingUIManager::Observer {
 public:
  
  
  static ClientSideDetectionHost* Create(content::WebContents* tab);
  virtual ~ClientSideDetectionHost();

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  
  
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;

  
  
  virtual void OnSafeBrowsingHit(
      const SafeBrowsingUIManager::UnsafeResource& resource) OVERRIDE;

  
  
  virtual void OnSafeBrowsingMatch(
      const SafeBrowsingUIManager::UnsafeResource& resource) OVERRIDE;

  virtual scoped_refptr<SafeBrowsingDatabaseManager> database_manager();

  
  
  bool DidPageReceiveSafeBrowsingMatch() const;

 protected:
  explicit ClientSideDetectionHost(content::WebContents* tab);

  
  virtual void WebContentsDestroyed(content::WebContents* tab) OVERRIDE;

  
  void set_safe_browsing_managers(
      SafeBrowsingUIManager* ui_manager,
      SafeBrowsingDatabaseManager* database_manager);

 private:
  friend class ClientSideDetectionHostTest;
  class ShouldClassifyUrlRequest;
  friend class ShouldClassifyUrlRequest;

  
  void OnPhishingDetectionDone(const std::string& verdict);

  
  
  
  void MaybeShowPhishingWarning(GURL phishing_url, bool is_phishing);

  
  
  
  void MaybeShowMalwareWarning(GURL original_url, GURL malware_url,
                               bool is_malware);

  
  
  
  void FeatureExtractionDone(bool success, ClientPhishingRequest* request);

  
  
  void MalwareFeatureExtractionDone(bool success,
                                    scoped_ptr<ClientMalwareRequest> request);

  
  void UpdateIPUrlMap(const std::string& ip,
                      const std::string& url,
                      const std::string& method,
                      const std::string& referrer,
                      const ResourceType::Type resource_type);

  
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  
  bool DidShowSBInterstitial() const;

  
  
  void set_client_side_detection_service(ClientSideDetectionService* service);

  
  bool MalwareKillSwitchIsOn();
  void SetMalwareKillSwitch(bool killswitch_on);

  
  ClientSideDetectionService* csd_service_;
  
  scoped_refptr<SafeBrowsingDatabaseManager> database_manager_;
  scoped_refptr<SafeBrowsingUIManager> ui_manager_;
  
  
  scoped_refptr<ShouldClassifyUrlRequest> classification_request_;
  
  scoped_ptr<BrowserFeatureExtractor> feature_extractor_;
  
  
  
  
  scoped_ptr<BrowseInfo> browse_info_;
  
  
  std::vector<GURL> cur_host_redirects_;
  
  std::string cur_host_;
  
  content::NotificationRegistrar registrar_;

  
  static const int kMaxIPsPerBrowse;
  
  static const int kMaxUrlsPerIP;

  base::WeakPtrFactory<ClientSideDetectionHost> weak_factory_;

  
  
  int unsafe_unique_page_id_;
  scoped_ptr<SafeBrowsingUIManager::UnsafeResource> unsafe_resource_;

  
  
  bool malware_killswitch_on_;

  
  
  bool malware_report_enabled_;

  
  
  bool malware_or_phishing_match_;

  DISALLOW_COPY_AND_ASSIGN(ClientSideDetectionHost);
};

}  

#endif  
