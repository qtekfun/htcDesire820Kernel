// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_PERMISSIONS_UPDATER_H__
#define CHROME_BROWSER_EXTENSIONS_PERMISSIONS_UPDATER_H__

#include <string>

#include "base/memory/ref_counted.h"

class Profile;

namespace base {
class DictionaryValue;
}

namespace extensions {

class Extension;
class ExtensionPrefs;
class PermissionSet;

class PermissionsUpdater {
 public:
  explicit PermissionsUpdater(Profile* profile);
  ~PermissionsUpdater();

  
  
  
  void AddPermissions(const Extension* extension,
                      const PermissionSet* permissions);

  
  
  void RemovePermissions(const Extension* extension,
                         const PermissionSet* permissions);

  
  
  void GrantActivePermissions(const Extension* extension);

  
  void UpdateActivePermissions(const Extension* extension,
                               const PermissionSet* permissions);

 private:
  enum EventType {
    ADDED,
    REMOVED,
  };

  
  void DispatchEvent(const std::string& extension_id,
                     const char* event_name,
                     const PermissionSet* changed_permissions);

  
  
  
  
  
  void NotifyPermissionsUpdated(EventType event_type,
                                const Extension* extension,
                                const PermissionSet* changed);

  
  ExtensionPrefs* GetExtensionPrefs();

  Profile* profile_;
};

}  

#endif  
