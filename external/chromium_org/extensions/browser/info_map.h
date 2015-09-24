// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_INFO_MAP_H_
#define EXTENSIONS_BROWSER_INFO_MAP_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "chrome/common/extensions/extension_set.h"
#include "extensions/browser/process_map.h"
#include "extensions/browser/quota_service.h"

namespace extensions {
class Extension;

class InfoMap : public base::RefCountedThreadSafe<InfoMap> {
 public:
  InfoMap();

  const ExtensionSet& extensions() const { return extensions_; }
  const ExtensionSet& disabled_extensions() const {
    return disabled_extensions_;
  }

  const extensions::ProcessMap& process_map() const;

  
  void AddExtension(const extensions::Extension* extension,
                    base::Time install_time,
                    bool incognito_enabled,
                    bool notifications_disabled);

  
  void RemoveExtension(const std::string& extension_id,
                       const extensions::UnloadedExtensionInfo::Reason reason);

  
  base::Time GetInstallTime(const std::string& extension_id) const;

  
  
  bool IsIncognitoEnabled(const std::string& extension_id) const;

  
  
  bool CanCrossIncognito(const extensions::Extension* extension) const;

  
  void RegisterExtensionProcess(const std::string& extension_id,
                                int process_id,
                                int site_instance_id);

  
  void UnregisterExtensionProcess(const std::string& extension_id,
                                  int process_id,
                                  int site_instance_id);
  void UnregisterAllExtensionsInProcess(int process_id);

  
  
  void GetExtensionsWithAPIPermissionForSecurityOrigin(
      const GURL& origin,
      int process_id,
      extensions::APIPermission::ID permission,
      ExtensionSet* extensions) const;

  
  
  bool SecurityOriginHasAPIPermission(const GURL& origin,
                                      int process_id,
                                      extensions::APIPermission::ID permission)
      const;

  QuotaService* GetQuotaService();

  
  
  void SetSigninProcess(int process_id);
  bool IsSigninProcess(int process_id) const;

  
  void SetNotificationsDisabled(const std::string& extension_id,
                               bool notifications_disabled);
  bool AreNotificationsDisabled(const std::string& extension_id)
      const;

 private:
  friend class base::RefCountedThreadSafe<InfoMap>;

  
  struct ExtraData;
  
  typedef std::map<std::string, ExtraData> ExtraDataMap;

  ~InfoMap();

  ExtensionSet extensions_;
  ExtensionSet disabled_extensions_;

  
  ExtraDataMap extra_data_;

  
  
  
  scoped_ptr<QuotaService> quota_service_;

  
  extensions::ProcessMap process_map_;

  int signin_process_id_;
};

}  

#endif  
