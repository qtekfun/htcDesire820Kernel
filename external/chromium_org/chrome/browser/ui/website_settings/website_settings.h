// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBSITE_SETTINGS_WEBSITE_SETTINGS_H_
#define CHROME_BROWSER_UI_WEBSITE_SETTINGS_WEBSITE_SETTINGS_H_

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/content_settings/tab_specific_content_settings.h"
#include "chrome/browser/history/history_service.h"
#include "chrome/common/content_settings.h"
#include "chrome/common/content_settings_types.h"
#include "ui/gfx/native_widget_types.h"
#include "url/gurl.h"

namespace content {
class CertStore;
struct SSLStatus;
}

class InfoBarService;
class HostContentSettingsMap;
class Profile;
class WebsiteSettingsUI;

class WebsiteSettings : public TabSpecificContentSettings::SiteDataObserver {
 public:
  
  enum SiteConnectionStatus {
    SITE_CONNECTION_STATUS_UNKNOWN = 0,      
    SITE_CONNECTION_STATUS_ENCRYPTED,        
    SITE_CONNECTION_STATUS_MIXED_CONTENT,    
    SITE_CONNECTION_STATUS_UNENCRYPTED,      
    SITE_CONNECTION_STATUS_ENCRYPTED_ERROR,  
    SITE_CONNECTION_STATUS_INTERNAL_PAGE,    
  };

  
  enum SiteIdentityStatus {
    
    SITE_IDENTITY_STATUS_UNKNOWN = 0,
    
    SITE_IDENTITY_STATUS_CERT,
    
    SITE_IDENTITY_STATUS_EV_CERT,
    
    
    SITE_IDENTITY_STATUS_CERT_REVOCATION_UNKNOWN,
    
    
    SITE_IDENTITY_STATUS_NO_CERT,
    
    SITE_IDENTITY_STATUS_ERROR,
    
    SITE_IDENTITY_STATUS_INTERNAL_PAGE,
    
    
    SITE_IDENTITY_STATUS_ADMIN_PROVIDED_CERT,
  };

  
  
  
  WebsiteSettings(WebsiteSettingsUI* ui,
                  Profile* profile,
                  TabSpecificContentSettings* tab_specific_content_settings,
                  InfoBarService* infobar_service,
                  const GURL& url,
                  const content::SSLStatus& ssl,
                  content::CertStore* cert_store);
  virtual ~WebsiteSettings();

  
  void OnSitePermissionChanged(ContentSettingsType type,
                               ContentSetting value);

  
  
  void OnGotVisitCountToHost(HistoryService::Handle handle,
                             bool found_visits,
                             int visit_count,
                             base::Time first_visit);

  
  void OnUIClosing();

  
  SiteConnectionStatus site_connection_status() const {
    return site_connection_status_;
  }

  SiteIdentityStatus site_identity_status() const {
    return site_identity_status_;
  }

  base::string16 site_connection_details() const {
    return site_connection_details_;
  }

  base::string16 site_identity_details() const {
    return site_identity_details_;
  }

  base::string16 organization_name() const {
    return organization_name_;
  }

  
  virtual void OnSiteDataAccessed() OVERRIDE;

 private:
  
  void Init(Profile* profile,
            const GURL& url,
            const content::SSLStatus& ssl);

  
  void PresentSitePermissions();

  
  void PresentSiteData();

  
  
  void PresentSiteIdentity();

  
  
  
  void PresentHistoryInfo(base::Time first_visit);

  
  
  
  
  WebsiteSettingsUI* ui_;

  
  InfoBarService* infobar_service_;

  
  
  bool show_info_bar_;

  
  
  GURL site_url_;

  
  SiteIdentityStatus site_identity_status_;

  
  
  int cert_id_;

  
  SiteConnectionStatus site_connection_status_;

  
  
  

  
  
  
  base::string16 site_identity_details_;

  
  
  
  
  base::string16 site_connection_details_;

  
  
  
  
  base::string16 organization_name_;

  
  content::CertStore* cert_store_;

  
  
  HostContentSettingsMap* content_settings_;

  
  CancelableRequestConsumer visit_count_request_consumer_;

  DISALLOW_COPY_AND_ASSIGN(WebsiteSettings);
};

#endif  
