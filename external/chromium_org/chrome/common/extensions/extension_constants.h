// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_EXTENSION_CONSTANTS_H_
#define CHROME_COMMON_EXTENSIONS_EXTENSION_CONSTANTS_H_

#include <string>

#include "base/basictypes.h"
#include "url/gurl.h"

namespace extension_urls {
  
  
  
  std::string GetWebstoreLaunchURL();

  
  
  std::string GetExtensionGalleryURL();

  
  
  
  std::string GetWebstoreItemDetailURLPrefix();

  
  
  GURL GetWebstoreItemJsonDataURL(const std::string& extension_id);

  
  
  
  
  
  
  GURL GetWebstoreJsonSearchUrl(const std::string& query,
                                const std::string& hl);

  
  GURL GetWebstoreSearchPageUrl(const std::string& query);

  
  GURL GetWebstoreUpdateUrl();

  
  
  bool IsWebstoreUpdateUrl(const GURL& update_url);

  
  bool IsBlacklistUpdateUrl(const GURL& url);

  
  
  extern const char kGalleryBrowsePrefix[];

  
  extern const char kWebstoreSourceField[];

  
  extern const char kLaunchSourceAppList[];
  extern const char kLaunchSourceAppListSearch[];
}  

namespace extension_misc {
  
  const int kUnknownWindowId = -1;

  
  const int kCurrentWindowId = -2;

  
  extern const char kBookmarkManagerId[];

  
  extern const char kChromeAppId[];

  
  extern const char kCloudPrintAppId[];

  
  extern const char kEnterpriseWebStoreAppId[];

  
  extern const char kGmailAppId[];

  
  extern const char kGoogleDocAppId[];

  
  extern const char kGooglePlayMusicAppId[];

  
  extern const char kGoogleSearchAppId[];

  
  extern const char kGoogleSheetsAppId[];

  
  extern const char kGoogleSlidesAppId[];

  
  extern const char kHTermAppId[];

  
  extern const char kHTermDevAppId[];

  
  extern const char kCroshBuiltinAppId[];

  
  extern const char kPdfExtensionId[];

  
  extern const char kQuickOfficeComponentExtensionId[];

  
  extern const char kQuickOfficeDevExtensionId[];

  
  extern const char kQuickOfficeExtensionId[];

  
  extern const char kSettingsAppId[];

  
  extern const char kStreamsPrivateTestExtensionId[];

  
  extern const char kWebStoreAppId[];

  
  extern const char kYoutubeAppId[];

  
  extern const char kIdentityApiUiAppId[];

  
  extern const char kInAppPaymentsSupportAppId[];

  
  extern const char kTipsAndTricksAppId[];

  
  
  
  const uint8 kSignatureAlgorithm[15] = {
    0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
    0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05, 0x00
  };

  
  extern const char kAppLaunchHistogram[];

  
  extern const char kPlatformAppLaunchHistogram[];

  
  enum AppLaunchBucket {
    
    APP_LAUNCH_NTP_APPS_MAXIMIZED,

    
    APP_LAUNCH_NTP_APPS_COLLAPSED,

    
    APP_LAUNCH_NTP_APPS_MENU,

    
    APP_LAUNCH_NTP_MOST_VISITED,

    
    APP_LAUNCH_NTP_RECENTLY_CLOSED,

    
    APP_LAUNCH_BOOKMARK_BAR,

    
    APP_LAUNCH_CONTENT_NAVIGATION,

    
    APP_LAUNCH_SESSION_RESTORE,

    
    APP_LAUNCH_AUTOLAUNCH,

    
    APP_LAUNCH_OMNIBOX_APP,

    
    APP_LAUNCH_OMNIBOX_LOCATION,

    
    APP_LAUNCH_OMNIBOX_INSTANT,

    
    APP_LAUNCH_EXTENSION_API,

    
    
    APP_LAUNCH_CMD_LINE_APP,

    
    APP_LAUNCH_CMD_LINE_URL,

    
    APP_LAUNCH_NTP_WEBSTORE,

    
    APP_LAUNCH_NTP_APP_RE_ENABLE,

    
    
    
    APP_LAUNCH_CMD_LINE_APP_LEGACY,

    
    APP_LAUNCH_NTP_WEBSTORE_FOOTER,

    
    APP_LAUNCH_NTP_WEBSTORE_PLUS_ICON,

    
    APP_LAUNCH_APP_LIST_MAIN,

    
    APP_LAUNCH_APP_LIST_SEARCH,

    
    APP_LAUNCH_APP_LIST_MAIN_CHROME,

    
    APP_LAUNCH_APP_LIST_MAIN_WEBSTORE,

    
    APP_LAUNCH_APP_LIST_SEARCH_CHROME,

    
    APP_LAUNCH_APP_LIST_SEARCH_WEBSTORE,

    APP_LAUNCH_BUCKET_BOUNDARY,
    APP_LAUNCH_BUCKET_INVALID
  };

  
  extern const char kChromeVoxExtensionId[];

#if defined(OS_CHROMEOS)
  
  
  extern const char kChromeVoxExtensionPath[];
  
  extern const char kConnectivityDiagnosticsPath[];
  extern const char kConnectivityDiagnosticsLauncherPath[];
  
  extern const char kSpeechSynthesisExtensionPath[];
  
  extern const char kSpeechSynthesisExtensionId[];
  
  extern const char kWallpaperManagerId[];
  
  extern const char kFirstRunDialogId[];
#endif

  
  
  enum CrxInstallCause {
    INSTALL_CAUSE_UNSET = 0,
    INSTALL_CAUSE_USER_DOWNLOAD,
    INSTALL_CAUSE_UPDATE,
    INSTALL_CAUSE_EXTERNAL_FILE,
    INSTALL_CAUSE_AUTOMATION,
    NUM_INSTALL_CAUSES
  };

  
  
  extern const char kAppStateNotInstalled[];
  extern const char kAppStateInstalled[];
  extern const char kAppStateDisabled[];
  extern const char kAppStateRunning[];
  extern const char kAppStateCannotRun[];
  extern const char kAppStateReadyToRun[];

  
  extern const char kMediaFileSystemPathPart[];

  
  
  enum ExtensionIcons {
    EXTENSION_ICON_GIGANTOR = 512,
    EXTENSION_ICON_EXTRA_LARGE = 256,
    EXTENSION_ICON_LARGE = 128,
    EXTENSION_ICON_MEDIUM = 48,
    EXTENSION_ICON_SMALL = 32,
    EXTENSION_ICON_SMALLISH = 24,
    EXTENSION_ICON_ACTION = 19,
    EXTENSION_ICON_BITTY = 16,
    EXTENSION_ICON_INVALID = 0,
  };

  
  extern const int kExtensionIconSizes[];
  extern const size_t kNumExtensionIconSizes;

  
  extern const int kExtensionActionIconSizes[];
  extern const size_t kNumExtensionActionIconSizes;

  
  extern const int kScriptBadgeIconSizes[];
  extern const size_t kNumScriptBadgeIconSizes;
}  

namespace extensions {
  
  
  
  
  enum LaunchType {
    LAUNCH_TYPE_PINNED,
    LAUNCH_TYPE_REGULAR,
    LAUNCH_TYPE_FULLSCREEN,
    LAUNCH_TYPE_WINDOW,

    
    
    
    LAUNCH_TYPE_DEFAULT = LAUNCH_TYPE_REGULAR
  };

  
  
  enum LaunchContainer {
    LAUNCH_CONTAINER_WINDOW,
    LAUNCH_CONTAINER_PANEL,
    LAUNCH_CONTAINER_TAB,
    
    
    LAUNCH_CONTAINER_NONE
  };
}  

#endif  
