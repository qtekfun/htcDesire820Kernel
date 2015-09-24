// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_EXTENSION_H_
#define CHROME_COMMON_EXTENSIONS_EXTENSION_H_
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/common/extensions/extension_constants.h"
#include "chrome/common/extensions/extension_extent.h"
#include "chrome/common/extensions/extension_icon_set.h"
#include "chrome/common/extensions/user_script.h"
#include "chrome/common/extensions/url_pattern.h"
#include "googleurl/src/gurl.h"
#include "ui/gfx/size.h"

class DictionaryValue;
class ExtensionAction;
class ExtensionResource;
class ExtensionSidebarDefaults;
class FileBrowserHandler;
class ListValue;
class SkBitmap;
class Version;

class Extension : public base::RefCountedThreadSafe<Extension> {
 public:
  typedef std::map<const std::string, GURL> URLOverrideMap;
  typedef std::vector<std::string> ScriptingWhitelist;
  typedef std::vector<linked_ptr<FileBrowserHandler> > FileBrowserHandlerList;

  
  
  
  
  enum Location {
    INVALID,
    INTERNAL,           
    EXTERNAL_PREF,      
    EXTERNAL_REGISTRY,  
                        
    LOAD,               
    COMPONENT,          
                        
                        
    EXTERNAL_PREF_DOWNLOAD,    
                               
    EXTERNAL_POLICY_DOWNLOAD,  
                               

    NUM_LOCATIONS
  };

  enum State {
    DISABLED = 0,
    ENABLED,
    
    
    EXTERNAL_EXTENSION_UNINSTALLED,
    NUM_STATES
  };

  enum InstallType {
    INSTALL_ERROR,
    DOWNGRADE,
    REINSTALL,
    UPGRADE,
    NEW_INSTALL
  };

  
  
  enum Icons {
    EXTENSION_ICON_LARGE = 128,
    EXTENSION_ICON_MEDIUM = 48,
    EXTENSION_ICON_SMALL = 32,
    EXTENSION_ICON_SMALLISH = 24,
    EXTENSION_ICON_BITTY = 16,
  };

  
  
  enum Type {
    TYPE_UNKNOWN = 0,
    TYPE_EXTENSION,
    TYPE_THEME,
    TYPE_USER_SCRIPT,
    TYPE_HOSTED_APP,
    TYPE_PACKAGED_APP
  };

  
  struct PluginInfo {
    FilePath path;  
    bool is_public;  
  };

  
  struct NaClModuleInfo {
    GURL url;
    std::string mime_type;
  };

  struct TtsVoice {
    std::string voice_name;
    std::string locale;
    std::string gender;
  };

  
  
  
  
  class PermissionMessage {
   public:
    
    
    
    enum MessageId {
      ID_UNKNOWN,
      ID_NONE,
      ID_BOOKMARKS,
      ID_GEOLOCATION,
      ID_BROWSING_HISTORY,
      ID_TABS,
      ID_MANAGEMENT,
      ID_DEBUGGER,
      ID_HOSTS_1,
      ID_HOSTS_2,
      ID_HOSTS_3,
      ID_HOSTS_4_OR_MORE,
      ID_HOSTS_ALL,
      ID_FULL_ACCESS,
      ID_ENUM_BOUNDARY
    };

    
    
    static PermissionMessage CreateFromMessageId(MessageId message_id);

    
    
    
    static PermissionMessage CreateFromHostList(
        const std::vector<std::string> hosts);

    
    
    MessageId message_id() const { return message_id_; }

    
    
    const string16& message() const { return message_; }

    
    bool operator<(const PermissionMessage& that) const {
      return message_id_ < that.message_id_;
    }

   private:
    PermissionMessage(MessageId message_id, string16 message_);

    
    
    
    
    
    static const int kMessageIds[];

    MessageId message_id_;
    string16 message_;
  };

  typedef std::vector<PermissionMessage> PermissionMessages;

  
  
  struct Permission {
    const char* const name;
    const PermissionMessage::MessageId message_id;
  };

  enum InitFromValueFlags {
    NO_FLAGS = 0,

    
    
    
    REQUIRE_KEY = 1 << 0,

    
    
    
    
    
    
    
    
    
    
    STRICT_ERROR_CHECKS = 1 << 1,

    
    
    
    ALLOW_FILE_ACCESS = 1 << 2,
  };

  static scoped_refptr<Extension> Create(const FilePath& path,
                                         Location location,
                                         const DictionaryValue& value,
                                         int flags,
                                         std::string* error);

  
  static GURL GalleryUpdateUrl(bool secure);

  
  
  
  static Location GetHigherPriorityLocation(Location loc1, Location loc2);

  
  
  static PermissionMessage::MessageId GetPermissionMessageId(
      const std::string& permission);

  
  
  PermissionMessages GetPermissionMessages() const;

  
  
  
  std::vector<string16> GetPermissionMessageStrings() const;

  
  
  
  
  
  
  
  
  
  static std::vector<std::string> GetDistinctHostsForDisplay(
      const URLPatternList& list);

  
  
  
  static bool IsElevatedHostList(
      const URLPatternList& old_list, const URLPatternList& new_list);

  
  static const int kIconSizes[];

  
  static const int kPageActionIconMaxSize;
  static const int kBrowserActionIconMaxSize;
  static const int kSidebarIconMaxSize;

  
  
  
  
  static const char kBackgroundPermission[];
  static const char kBookmarkPermission[];
  static const char kContentSettingsPermission[];
  static const char kContextMenusPermission[];
  static const char kCookiePermission[];
  static const char kChromeosInfoPrivatePermissions[];
  static const char kDebuggerPermission[];
  static const char kExperimentalPermission[];
  static const char kFileBrowserHandlerPermission[];
  static const char kFileBrowserPrivatePermission[];
  static const char kGeolocationPermission[];
  static const char kHistoryPermission[];
  static const char kIdlePermission[];
  static const char kManagementPermission[];
  static const char kNotificationPermission[];
  static const char kProxyPermission[];
  static const char kTabPermission[];
  static const char kUnlimitedStoragePermission[];
  static const char kWebstorePrivatePermission[];

  static const Permission kPermissions[];
  static const size_t kNumPermissions;
  static const char* const kHostedAppPermissionNames[];
  static const size_t kNumHostedAppPermissions;
  static const char* const kComponentPrivatePermissionNames[];
  static const size_t kNumComponentPrivatePermissions;

  
  
  static const char kOldUnlimitedStoragePermission[];

  
  static const int kValidWebExtentSchemes;

  
  static const int kValidHostPermissionSchemes;

  
  
  static bool IsHostedAppPermission(const std::string& permission);

  
  static const FilePath::CharType kManifestFilename[];

  
  static const FilePath::CharType kLocaleFolder[];

  
  static const FilePath::CharType kMessagesFilename[];

#if defined(OS_WIN)
  static const char kExtensionRegistryPath[];
#endif

  
  static const size_t kIdSize;

  
  static const char kMimeType[];

  
  static bool IdIsValid(const std::string& id);

  
  
  static std::string GenerateIdForPath(const FilePath& file_name);

  
  static bool IsExtension(const FilePath& file_name);

  
  static inline bool IsExternalLocation(Location location) {
    return location == Extension::EXTERNAL_PREF ||
           location == Extension::EXTERNAL_REGISTRY ||
           location == Extension::EXTERNAL_PREF_DOWNLOAD ||
           location == Extension::EXTERNAL_POLICY_DOWNLOAD;
  }

  
  static inline bool IsAutoUpdateableLocation(Location location) {
    
    return location == Extension::INTERNAL ||
           IsExternalLocation(location);
  }

  
  
  
  static inline bool UserMayDisable(Location location) {
    return location != Extension::EXTERNAL_POLICY_DOWNLOAD &&
           location != Extension::COMPONENT;
  }

  
  
  
  
  
  
  static inline bool ShouldDoStrictErrorChecking(Location location) {
    return location == Extension::LOAD ||
           location == Extension::COMPONENT;
  }

  
  
  static inline bool ShouldAlwaysAllowFileAccess(Location location) {
    return location == Extension::LOAD;
  }

  
  Type GetType() const;

  
  
  
  
  
  static GURL GetResourceURL(const GURL& extension_url,
                             const std::string& relative_path);
  GURL GetResourceURL(const std::string& relative_path) const {
    return GetResourceURL(url(), relative_path);
  }

  
  
  ExtensionResource GetResource(const std::string& relative_path) const;

  
  ExtensionResource GetResource(const FilePath& relative_path) const;

  
  
  
  
  static bool ParsePEMKeyBytes(const std::string& input, std::string* output);

  
  static bool ProducePEM(const std::string& input, std::string* output);

  
  
  static bool GenerateId(const std::string& input, std::string* output);

  
  
  static bool FormatPEMForFileOutput(const std::string& input,
                                     std::string* output,
                                     bool is_public);

  
  
  
  static bool IsPrivilegeIncrease(const bool granted_full_access,
                                  const std::set<std::string>& granted_apis,
                                  const ExtensionExtent& granted_extent,
                                  const Extension* new_extension);

  
  
  
  
  static void DecodeIcon(const Extension* extension,
                         Icons icon_size,
                         scoped_ptr<SkBitmap>* result);

  
  
  
  
  static void DecodeIconFromPath(const FilePath& icon_path,
                                 Icons icon_size,
                                 scoped_ptr<SkBitmap>* result);

  
  
  static const SkBitmap& GetDefaultIcon(bool is_app);

  
  static GURL GetBaseURLFromExtensionId(const std::string& extension_id);

  
  
  
  
  
  static std::string ChromeStoreLaunchURL();

  
  static void SetScriptingWhitelist(const ScriptingWhitelist& whitelist);
  static const ScriptingWhitelist* GetScriptingWhitelist();

  
  static bool HasApiPermission(const std::set<std::string>& api_permissions,
                               const std::string& function_name);

  
  
  
  static bool HasEffectiveAccessToAllHosts(
      const ExtensionExtent& effective_host_permissions,
      const std::set<std::string>& api_permissions);

  bool HasApiPermission(const std::string& function_name) const {
    return HasApiPermission(this->api_permissions(), function_name);
  }

  const ExtensionExtent& GetEffectiveHostPermissions() const {
    return effective_host_permissions_;
  }

  
  
  
  
  bool CanSpecifyHostPermission(const URLPattern& pattern) const;

  
  bool HasHostPermission(const GURL& url) const;

  
  
  
  
  
  bool HasEffectiveAccessToAllHosts() const;

  
  
  bool HasFullPermissions() const;

  
  bool ShowConfigureContextMenus() const;

  
  
  
  GURL GetHomepageURL() const;

  
  
  std::set<FilePath> GetBrowserImages() const;

  
  ExtensionResource GetIconResource(
      int size, ExtensionIconSet::MatchType match_type) const;
  GURL GetIconURL(int size, ExtensionIconSet::MatchType match_type) const;

  
  GURL GetFullLaunchURL() const;

  
  
  
  
  void SetCachedImage(const ExtensionResource& source,
                      const SkBitmap& image,
                      const gfx::Size& original_size) const;
  bool HasCachedImage(const ExtensionResource& source,
                      const gfx::Size& max_size) const;
  SkBitmap GetCachedImage(const ExtensionResource& source,
                          const gfx::Size& max_size) const;

  
  
  
  
  
  
  
  bool CanExecuteScriptOnPage(const GURL& page_url,
                              const UserScript* script,
                              std::string* error) const;

  
  
  bool CanExecuteScriptEverywhere() const;

  
  
  
  
  bool CanCaptureVisiblePage(const GURL& page_url, std::string* error) const;

  
  
  bool UpdatesFromGallery() const;

  
  bool OverlapsWithOrigin(const GURL& origin) const;

  

  const FilePath& path() const { return path_; }
  const GURL& url() const { return extension_url_; }
  Location location() const { return location_; }
  const std::string& id() const { return id_; }
  const Version* version() const { return version_.get(); }
  const std::string VersionString() const;
  const std::string& name() const { return name_; }
  const std::string& public_key() const { return public_key_; }
  const std::string& description() const { return description_; }
  bool converted_from_user_script() const {
    return converted_from_user_script_;
  }
  const UserScriptList& content_scripts() const { return content_scripts_; }
  ExtensionAction* page_action() const { return page_action_.get(); }
  ExtensionAction* browser_action() const { return browser_action_.get(); }
  ExtensionSidebarDefaults* sidebar_defaults() const {
    return sidebar_defaults_.get();
  }
  const FileBrowserHandlerList* file_browser_handlers() const {
    return file_browser_handlers_.get();
  }
  const std::vector<PluginInfo>& plugins() const { return plugins_; }
  const std::vector<NaClModuleInfo>& nacl_modules() const {
    return nacl_modules_;
  }
  const GURL& background_url() const { return background_url_; }
  const GURL& options_url() const { return options_url_; }
  const GURL& devtools_url() const { return devtools_url_; }
  const std::vector<GURL>& toolstrips() const { return toolstrips_; }
  const std::set<std::string>& api_permissions() const {
    return api_permissions_;
  }
  const URLPatternList& host_permissions() const { return host_permissions_; }
  const GURL& update_url() const { return update_url_; }
  const ExtensionIconSet& icons() const { return icons_; }
  const DictionaryValue* manifest_value() const {
    return manifest_value_.get();
  }
  const std::string default_locale() const { return default_locale_; }
  const URLOverrideMap& GetChromeURLOverrides() const {
    return chrome_url_overrides_;
  }
  const std::string omnibox_keyword() const { return omnibox_keyword_; }
  bool incognito_split_mode() const { return incognito_split_mode_; }
  const std::vector<TtsVoice>& tts_voices() const { return tts_voices_; }

  bool wants_file_access() const { return wants_file_access_; }

  
  bool is_app() const { return is_app_; }
  bool is_hosted_app() const { return is_app() && !web_extent().is_empty(); }
  bool is_packaged_app() const { return is_app() && web_extent().is_empty(); }
  bool is_storage_isolated() const { return is_app() && is_storage_isolated_; }
  const ExtensionExtent& web_extent() const { return extent_; }
  const std::string& launch_local_path() const { return launch_local_path_; }
  const std::string& launch_web_url() const { return launch_web_url_; }
  extension_misc::LaunchContainer launch_container() const {
    return launch_container_;
  }
  int launch_width() const { return launch_width_; }
  int launch_height() const { return launch_height_; }

  
  bool is_theme() const { return is_theme_; }
  DictionaryValue* GetThemeImages() const { return theme_images_.get(); }
  DictionaryValue* GetThemeColors() const {return theme_colors_.get(); }
  DictionaryValue* GetThemeTints() const { return theme_tints_.get(); }
  DictionaryValue* GetThemeDisplayProperties() const {
    return theme_display_properties_.get();
  }

 private:
  friend class base::RefCountedThreadSafe<Extension>;

  
  
  
  typedef std::pair<FilePath, std::string> ImageCacheKey;
  typedef std::map<ImageCacheKey, SkBitmap> ImageCache;

  
  
  static FilePath MaybeNormalizePath(const FilePath& path);

  
  
  
  
  
  
  
  
  
  
  
  
  
  static std::vector<std::string> GetDistinctHosts(
      const URLPatternList& host_patterns, bool include_rcd);

  Extension(const FilePath& path, Location location);
  ~Extension();

  
  bool InitFromValue(const DictionaryValue& value, int flags,
                     std::string* error);

  
  
  
  SkBitmap* GetCachedImageImpl(const ExtensionResource& source,
                               const gfx::Size& max_size) const;

  
  
  bool LoadUserScriptHelper(const DictionaryValue* content_script,
                            int definition_index,
                            int flags,
                            std::string* error,
                            UserScript* result);

  
  
  bool LoadGlobsHelper(const DictionaryValue* content_script,
                       int content_script_index,
                       const char* globs_property_name,
                       std::string* error,
                       void(UserScript::*add_method)(const std::string& glob),
                       UserScript *instance);

  
  bool LoadIsApp(const DictionaryValue* manifest, std::string* error);
  bool LoadExtent(const DictionaryValue* manifest,
                  const char* key,
                  ExtensionExtent* extent,
                  const char* list_error,
                  const char* value_error,
                  URLPattern::ParseOption parse_strictness,
                  std::string* error);
  bool LoadLaunchContainer(const DictionaryValue* manifest, std::string* error);
  bool LoadLaunchURL(const DictionaryValue* manifest, std::string* error);
  bool LoadAppIsolation(const DictionaryValue* manifest, std::string* error);
  bool EnsureNotHybridApp(const DictionaryValue* manifest, std::string* error);

  
  
  ExtensionAction* LoadExtensionActionHelper(
      const DictionaryValue* extension_action, std::string* error);

  
  FileBrowserHandlerList* LoadFileBrowserHandlers(
      const ListValue* extension_actions, std::string* error);
  
  FileBrowserHandler* LoadFileBrowserHandler(
      const DictionaryValue* file_browser_handlers, std::string* error);

  
  
  ExtensionSidebarDefaults* LoadExtensionSidebarDefaults(
      const DictionaryValue* sidebar, std::string* error);

  
  
  void InitEffectiveHostPermissions();

  
  
  bool HasMultipleUISurfaces() const;

  
  
  bool ContainsNonThemeKeys(const DictionaryValue& source) const;

  
  
  bool IsAPIPermission(const std::string& permission) const;

  
  
  bool IsComponentOnlyPermission(const std::string& permission) const;

  
  
  
  
  std::set<PermissionMessage> GetSimplePermissionMessages() const;

  
  
  mutable ImageCache image_cache_;

  
  
  
  
  std::string id_;

  
  
  
  
  std::string name_;

  
  FilePath path_;

  
  std::string default_locale_;

  
  
  bool incognito_split_mode_;

  
  ExtensionExtent extent_;

  
  
  
  
  
  ExtensionExtent effective_host_permissions_;

  
  std::set<std::string> api_permissions_;

  
  ExtensionIconSet icons_;

  
  GURL extension_url_;

  
  Location location_;

  
  scoped_ptr<Version> version_;

  
  std::string description_;

  
  
  bool converted_from_user_script_;

  
  UserScriptList content_scripts_;

  
  scoped_ptr<ExtensionAction> page_action_;

  
  scoped_ptr<ExtensionAction> browser_action_;

  
  scoped_ptr<FileBrowserHandlerList> file_browser_handlers_;

  
  scoped_ptr<ExtensionSidebarDefaults> sidebar_defaults_;

  
  std::vector<PluginInfo> plugins_;

  
  std::vector<NaClModuleInfo> nacl_modules_;

  
  
  GURL background_url_;

  
  GURL options_url_;

  
  GURL devtools_url_;

  
  std::vector<GURL> toolstrips_;

  
  std::string public_key_;

  
  scoped_ptr<DictionaryValue> theme_images_;

  
  scoped_ptr<DictionaryValue> theme_colors_;

  
  scoped_ptr<DictionaryValue> theme_tints_;

  
  scoped_ptr<DictionaryValue> theme_display_properties_;

  
  bool is_theme_;

  
  URLPatternList host_permissions_;

  
  
  GURL homepage_url_;

  
  GURL update_url_;

  
  scoped_ptr<DictionaryValue> manifest_value_;

  
  
  URLOverrideMap chrome_url_overrides_;

  
  bool is_app_;

  
  bool is_storage_isolated_;

  
  std::string launch_local_path_;

  
  
  std::string launch_web_url_;

  
  
  
  
  
  extension_misc::LaunchContainer launch_container_;

  
  
  int launch_width_;
  int launch_height_;

  
  std::string omnibox_keyword_;

  
  std::vector<TtsVoice> tts_voices_;

  
  
  
  bool wants_file_access_;

  FRIEND_TEST_ALL_PREFIXES(ExtensionServiceTest,
                           UpdateExtensionPreservesLocation);
  FRIEND_TEST_ALL_PREFIXES(ExtensionTest, LoadPageActionHelper);
  FRIEND_TEST_ALL_PREFIXES(ExtensionTest, InitFromValueInvalid);
  FRIEND_TEST_ALL_PREFIXES(ExtensionTest, InitFromValueValid);
  FRIEND_TEST_ALL_PREFIXES(ExtensionTest, InitFromValueValidNameInRTL);
  FRIEND_TEST_ALL_PREFIXES(TabStripModelTest, Apps);

  DISALLOW_COPY_AND_ASSIGN(Extension);
};

typedef std::vector< scoped_refptr<const Extension> > ExtensionList;
typedef std::set<std::string> ExtensionIdSet;

struct ExtensionInfo {
  ExtensionInfo(const DictionaryValue* manifest,
                const std::string& id,
                const FilePath& path,
                Extension::Location location);
  ~ExtensionInfo();

  scoped_ptr<DictionaryValue> extension_manifest;
  std::string extension_id;
  FilePath extension_path;
  Extension::Location extension_location;

 private:
  DISALLOW_COPY_AND_ASSIGN(ExtensionInfo);
};

struct UninstalledExtensionInfo {
  explicit UninstalledExtensionInfo(const Extension& extension);
  ~UninstalledExtensionInfo();

  std::string extension_id;
  std::set<std::string> extension_api_permissions;
  Extension::Type extension_type;
  GURL update_url;
};

struct UnloadedExtensionInfo {
  enum Reason {
    DISABLE,    
    UPDATE,     
    UNINSTALL,  
  };

  Reason reason;

  
  bool already_disabled;

  
  const Extension* extension;

  UnloadedExtensionInfo(const Extension* extension, Reason reason);
};

#endif  
