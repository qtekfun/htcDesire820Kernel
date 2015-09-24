// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_EXTENSION_H_
#define EXTENSIONS_COMMON_EXTENSION_H_

#include <algorithm>
#include <iosfwd>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "extensions/common/extension_resource.h"
#include "extensions/common/install_warning.h"
#include "extensions/common/manifest.h"
#include "extensions/common/permissions/api_permission.h"
#include "extensions/common/url_pattern.h"
#include "extensions/common/url_pattern_set.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/gfx/size.h"
#include "url/gurl.h"

class ExtensionAction;
class SkBitmap;

namespace base {
class DictionaryValue;
class Version;
}

namespace gfx {
class ImageSkia;
}

namespace extensions {
class PermissionsData;
class APIPermissionSet;
class ManifestPermissionSet;
class PermissionSet;

class Extension : public base::RefCountedThreadSafe<Extension> {
 public:
  struct ManifestData;

  typedef std::map<const std::string, linked_ptr<ManifestData> >
      ManifestDataMap;

  enum State {
    DISABLED = 0,
    ENABLED,
    
    
    EXTERNAL_EXTENSION_UNINSTALLED,
    
    
    ENABLED_COMPONENT,
    NUM_STATES
  };

  
  enum DeprecatedDisableReason {
    DEPRECATED_DISABLE_UNKNOWN,
    DEPRECATED_DISABLE_USER_ACTION,
    DEPRECATED_DISABLE_PERMISSIONS_INCREASE,
    DEPRECATED_DISABLE_RELOAD,
    DEPRECATED_DISABLE_LAST,  
  };

  enum DisableReason {
    DISABLE_NONE = 0,
    DISABLE_USER_ACTION = 1 << 0,
    DISABLE_PERMISSIONS_INCREASE = 1 << 1,
    DISABLE_RELOAD = 1 << 2,
    DISABLE_UNSUPPORTED_REQUIREMENT = 1 << 3,
    DISABLE_SIDELOAD_WIPEOUT = 1 << 4,
    DISABLE_UNKNOWN_FROM_SYNC = 1 << 5,
    DISABLE_PERMISSIONS_CONSENT = 1 << 6,  
    DISABLE_KNOWN_DISABLED = 1 << 7,

    
    DISABLE_NOT_VERIFIED = 1 << 8,
  };

  enum InstallType {
    INSTALL_ERROR,
    DOWNGRADE,
    REINSTALL,
    UPGRADE,
    NEW_INSTALL
  };

  
  
  
  struct ManifestData {
    virtual ~ManifestData() {}
  };

  enum InitFromValueFlags {
    NO_FLAGS = 0,

    
    
    
    REQUIRE_KEY = 1 << 0,

    
    
    
    
    REQUIRE_MODERN_MANIFEST_VERSION = 1 << 1,

    
    
    
    ALLOW_FILE_ACCESS = 1 << 2,

    
    
    FROM_WEBSTORE = 1 << 3,

    
    
    FROM_BOOKMARK = 1 << 4,

    
    
    
    FOLLOW_SYMLINKS_ANYWHERE = 1 << 5,

    
    
    ERROR_ON_PRIVATE_KEY = 1 << 6,

    
    
    WAS_INSTALLED_BY_DEFAULT = 1 << 7,

    
    REQUIRE_PERMISSIONS_CONSENT = 1 << 8,

    
    
    IS_EPHEMERAL = 1 << 9,
  };

  static scoped_refptr<Extension> Create(const base::FilePath& path,
                                         Manifest::Location location,
                                         const base::DictionaryValue& value,
                                         int flags,
                                         std::string* error);

  
  
  static scoped_refptr<Extension> Create(const base::FilePath& path,
                                         Manifest::Location location,
                                         const base::DictionaryValue& value,
                                         int flags,
                                         const std::string& explicit_id,
                                         std::string* error);

  
  static const int kValidWebExtentSchemes;

  
  static const int kValidHostPermissionSchemes;

  
  static const char kMimeType[];

  
  static bool IdIsValid(const std::string& id);

  
  Manifest::Type GetType() const;

  
  
  
  
  
  static GURL GetResourceURL(const GURL& extension_url,
                             const std::string& relative_path);
  GURL GetResourceURL(const std::string& relative_path) const {
    return GetResourceURL(url(), relative_path);
  }

  
  bool ResourceMatches(const URLPatternSet& pattern_set,
                       const std::string& resource) const;

  
  
  ExtensionResource GetResource(const std::string& relative_path) const;

  
  ExtensionResource GetResource(const base::FilePath& relative_path) const;

  
  
  
  
  static bool ParsePEMKeyBytes(const std::string& input, std::string* output);

  
  static bool ProducePEM(const std::string& input, std::string* output);

  
  
  static bool FormatPEMForFileOutput(const std::string& input,
                                     std::string* output,
                                     bool is_public);

  
  static GURL GetBaseURLFromExtensionId(const std::string& extension_id);

  
  
  bool HasAPIPermission(APIPermission::ID permission) const;
  bool HasAPIPermission(const std::string& permission_name) const;
  scoped_refptr<const PermissionSet> GetActivePermissions() const;

  
  bool ShowConfigureContextMenus() const;

  
  bool OverlapsWithOrigin(const GURL& origin) const;

  
  
  bool RequiresSortOrdinal() const;

  
  bool ShouldDisplayInAppLauncher() const;

  
  bool ShouldDisplayInNewTabPage() const;

  
  
  bool ShouldDisplayInExtensionSettings() const;

  
  
  
  bool ShouldNotBeVisible() const;

  
  
  ManifestData* GetManifestData(const std::string& key) const;

  
  
  
  void SetManifestData(const std::string& key, ManifestData* data);

  

  const base::FilePath& path() const { return path_; }
  const GURL& url() const { return extension_url_; }
  Manifest::Location location() const;
  const std::string& id() const;
  const base::Version* version() const { return version_.get(); }
  const std::string VersionString() const;
  const std::string& name() const { return name_; }
  const std::string& short_name() const { return short_name_; }
  const std::string& non_localized_name() const { return non_localized_name_; }
  
  
  
  const std::string& public_key() const { return public_key_; }
  const std::string& description() const { return description_; }
  int manifest_version() const { return manifest_version_; }
  bool converted_from_user_script() const {
    return converted_from_user_script_;
  }
  PermissionsData* permissions_data() { return permissions_data_.get(); }
  const PermissionsData* permissions_data() const {
    return permissions_data_.get();
  }

  
  void AddInstallWarning(const InstallWarning& new_warning);
  void AddInstallWarnings(const std::vector<InstallWarning>& new_warnings);
  const std::vector<InstallWarning>& install_warnings() const {
    return install_warnings_;
  }
  const extensions::Manifest* manifest() const {
    return manifest_.get();
  }
  bool wants_file_access() const { return wants_file_access_; }
  
  
  
  void set_wants_file_access(bool wants_file_access) {
    wants_file_access_ = wants_file_access;
  }
  int creation_flags() const { return creation_flags_; }
  bool from_webstore() const { return (creation_flags_ & FROM_WEBSTORE) != 0; }
  bool from_bookmark() const { return (creation_flags_ & FROM_BOOKMARK) != 0; }
  bool was_installed_by_default() const {
    return (creation_flags_ & WAS_INSTALLED_BY_DEFAULT) != 0;
  }
  bool is_ephemeral() const { return (creation_flags_ & IS_EPHEMERAL) != 0; }

  
  bool is_app() const;
  bool is_platform_app() const;
  bool is_hosted_app() const;
  bool is_legacy_packaged_app() const;
  bool is_extension() const;
  bool can_be_incognito_enabled() const;
  bool force_incognito_enabled() const;

  void AddWebExtentPattern(const URLPattern& pattern);
  const URLPatternSet& web_extent() const { return extent_; }

  
  bool is_theme() const;

 private:
  friend class base::RefCountedThreadSafe<Extension>;

  
  
  static bool InitExtensionID(extensions::Manifest* manifest,
                              const base::FilePath& path,
                              const std::string& explicit_id,
                              int creation_flags,
                              string16* error);

  Extension(const base::FilePath& path,
            scoped_ptr<extensions::Manifest> manifest);
  virtual ~Extension();

  
  
  
  
  
  
  bool InitFromValue(int flags, string16* error);

  
  

  bool LoadRequiredFeatures(string16* error);
  bool LoadName(string16* error);
  bool LoadVersion(string16* error);

  bool LoadAppFeatures(string16* error);
  bool LoadExtent(const char* key,
                  URLPatternSet* extent,
                  const char* list_error,
                  const char* value_error,
                  string16* error);

  bool LoadSharedFeatures(string16* error);
  bool LoadDescription(string16* error);
  bool LoadManifestVersion(string16* error);
  bool LoadShortName(string16* error);

  bool CheckMinimumChromeVersion(string16* error) const;

  
  
  
  
  std::string name_;

  
  
  std::string non_localized_name_;

  
  
  
  
  std::string short_name_;

  
  
  
  
  
  
  int manifest_version_;

  
  base::FilePath path_;

  
  URLPatternSet extent_;

  scoped_ptr<PermissionsData> permissions_data_;

  
  std::vector<InstallWarning> install_warnings_;

  
  GURL extension_url_;

  
  scoped_ptr<base::Version> version_;

  
  std::string description_;

  
  
  bool converted_from_user_script_;

  
  std::string public_key_;

  
  scoped_ptr<Manifest> manifest_;

  
  ManifestDataMap manifest_data_;

  
  bool finished_parsing_manifest_;

  
  
  
  base::ThreadChecker thread_checker_;

  
  bool display_in_launcher_;

  
  bool display_in_new_tab_page_;

  
  
  
  bool wants_file_access_;

  
  int creation_flags_;

  DISALLOW_COPY_AND_ASSIGN(Extension);
};

typedef std::vector<scoped_refptr<const Extension> > ExtensionList;
typedef std::set<std::string> ExtensionIdSet;
typedef std::vector<std::string> ExtensionIdList;

struct ExtensionInfo {
  ExtensionInfo(const base::DictionaryValue* manifest,
                const std::string& id,
                const base::FilePath& path,
                Manifest::Location location);
  ~ExtensionInfo();

  scoped_ptr<base::DictionaryValue> extension_manifest;
  std::string extension_id;
  base::FilePath extension_path;
  Manifest::Location extension_location;

 private:
  DISALLOW_COPY_AND_ASSIGN(ExtensionInfo);
};

struct InstalledExtensionInfo {
  
  const Extension* extension;

  
  bool is_update;

  
  
  std::string old_name;

  InstalledExtensionInfo(const Extension* extension,
                         bool is_update,
                         const std::string& old_name);
};

struct UnloadedExtensionInfo {
  enum Reason {
    REASON_DISABLE,    
    REASON_UPDATE,     
    REASON_UNINSTALL,  
    REASON_TERMINATE,  
    REASON_BLACKLIST,  
  };

  Reason reason;

  
  const Extension* extension;

  UnloadedExtensionInfo(const Extension* extension, Reason reason);
};

struct UpdatedExtensionPermissionsInfo {
  enum Reason {
    ADDED,    
    REMOVED,  
  };

  Reason reason;

  
  const Extension* extension;

  
  
  
  const PermissionSet* permissions;

  UpdatedExtensionPermissionsInfo(
      const Extension* extension,
      const PermissionSet* permissions,
      Reason reason);
};

}  

#endif  
