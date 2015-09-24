// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBSITE_SETTINGS_WEBSITE_SETTINGS_UI_H_
#define CHROME_BROWSER_UI_WEBSITE_SETTINGS_WEBSITE_SETTINGS_UI_H_

#include <string>
#include <vector>

#include "base/strings/string16.h"
#include "chrome/browser/ui/website_settings/website_settings.h"
#include "chrome/common/content_settings.h"
#include "chrome/common/content_settings_types.h"
#include "ui/gfx/native_widget_types.h"


class GURL;
class Profile;
class WebsiteSettings;
namespace content {
struct SSLStatus;
}

namespace gfx {
class Image;
}

class WebsiteSettingsUI {
 public:
  
  
  enum TabId {
    TAB_ID_PERMISSIONS = 0,
    TAB_ID_CONNECTION,
    NUM_TAB_IDS,
  };

  
  
  struct CookieInfo {
    CookieInfo();

    
    std::string cookie_source;
    
    int allowed;
    
    int blocked;
  };

  
  
  struct PermissionInfo {
    PermissionInfo();

    
    ContentSettingsType type;
    
    ContentSetting setting;
    
    ContentSetting default_setting;
    
    content_settings::SettingSource source;
  };

  
  
  struct IdentityInfo {
    IdentityInfo();

    
    std::string site_identity;
    
    WebsiteSettings::SiteIdentityStatus identity_status;
    
    base::string16 GetIdentityStatusText() const;
    
    
    std::string identity_status_description;
    
    int cert_id;
    
    WebsiteSettings::SiteConnectionStatus connection_status;
    
    
    std::string connection_status_description;
  };

  typedef std::vector<CookieInfo> CookieInfoList;
  typedef std::vector<PermissionInfo> PermissionInfoList;

  virtual ~WebsiteSettingsUI();

  
  static base::string16 PermissionTypeToUIString(ContentSettingsType type);

  
  
  
  static base::string16 PermissionValueToUIString(ContentSetting value);

  
  
  
  static base::string16 PermissionActionToUIString(
      ContentSetting setting,
      ContentSetting default_setting,
      content_settings::SettingSource source);

  
  static int GetPermissionIconID(ContentSettingsType type,
                                 ContentSetting setting);

  
  static const gfx::Image& GetPermissionIcon(ContentSettingsType type,
                                             ContentSetting setting);

  
  static int GetIdentityIconID(WebsiteSettings::SiteIdentityStatus status);

  
  static const gfx::Image& GetIdentityIcon(
      WebsiteSettings::SiteIdentityStatus status);

  
  static int GetConnectionIconID(
      WebsiteSettings::SiteConnectionStatus status);

  
  static const gfx::Image& GetConnectionIcon(
      WebsiteSettings::SiteConnectionStatus status);

  
  static int GetFirstVisitIconID(const base::string16& first_visit);

  
  static const gfx::Image& GetFirstVisitIcon(const base::string16& first_visit);

  
  virtual void SetCookieInfo(const CookieInfoList& cookie_info_list) = 0;

  
  virtual void SetPermissionInfo(
      const PermissionInfoList& permission_info_list) = 0;

  
  virtual void SetIdentityInfo(const IdentityInfo& identity_info) = 0;

  
  virtual void SetFirstVisit(const base::string16& first_visit) = 0;

  
  virtual void SetSelectedTab(TabId tab_id) = 0;
};

typedef WebsiteSettingsUI::CookieInfoList CookieInfoList;
typedef WebsiteSettingsUI::PermissionInfoList PermissionInfoList;

#endif  
