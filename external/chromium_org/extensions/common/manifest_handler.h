// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_MANIFEST_HANDLER_H_
#define EXTENSIONS_COMMON_MANIFEST_HANDLER_H_

#include <set>
#include <string>
#include <vector>

#include "base/lazy_instance.h"
#include "base/memory/linked_ptr.h"
#include "base/strings/string16.h"
#include "extensions/common/manifest.h"

namespace extensions {
class Extension;
class ManifestPermission;
class ManifestPermissionSet;

class ManifestHandler {
 public:
  ManifestHandler();
  virtual ~ManifestHandler();

  
  
  
  virtual bool Parse(Extension* extension, string16* error) = 0;

  
  
  
  
  
  
  
  virtual bool Validate(const Extension* extension,
                        std::string* error,
                        std::vector<InstallWarning>* warnings) const;

  
  
  
  
  
  
  virtual bool AlwaysParseForType(Manifest::Type type) const;

  
  virtual bool AlwaysValidateForType(Manifest::Type type) const;

  
  
  
  virtual const std::vector<std::string> PrerequisiteKeys() const;

  
  
  
  
  
  
  void Register();

  
  
  
  
  virtual ManifestPermission* CreatePermission();

  
  
  
  
  virtual ManifestPermission* CreateInitialRequiredPermission(
      const Extension* extension);

  
  
  static void FinalizeRegistration();

  static bool IsRegistrationFinalized();

  
  
  static bool ParseExtension(Extension* extension, string16* error);

  
  static bool ValidateExtension(const Extension* extension,
                                std::string* error,
                                std::vector<InstallWarning>* warnings);

  
  
  
  static ManifestPermission* CreatePermission(const std::string& key);

  
  
  
  
  
  
  static void AddExtensionInitialRequiredPermissions(
      const Extension* extension, ManifestPermissionSet* permission_set);

 protected:
  
  
  static const std::vector<std::string> SingleKey(const std::string& key);

 private:
  
  virtual const std::vector<std::string> Keys() const = 0;
};

class ManifestHandlerRegistry {
 private:
  friend class ManifestHandler;
  friend class ScopedTestingManifestHandlerRegistry;
  friend struct base::DefaultLazyInstanceTraits<ManifestHandlerRegistry>;

  ManifestHandlerRegistry();
  ~ManifestHandlerRegistry();

  void Finalize();

  void RegisterManifestHandler(const std::string& key,
                               linked_ptr<ManifestHandler> handler);
  bool ParseExtension(Extension* extension, string16* error);
  bool ValidateExtension(const Extension* extension,
                         std::string* error,
                         std::vector<InstallWarning>* warnings);

  ManifestPermission* CreatePermission(const std::string& key);

  void AddExtensionInitialRequiredPermissions(
      const Extension* extension,
      ManifestPermissionSet* permission_set);

  
  
  static ManifestHandlerRegistry* SetForTesting(
      ManifestHandlerRegistry* new_registry);

  typedef std::map<std::string, linked_ptr<ManifestHandler> >
      ManifestHandlerMap;
  typedef std::map<ManifestHandler*, int> ManifestHandlerPriorityMap;

  
  
  
  
  void SortManifestHandlers();

  
  ManifestHandlerMap handlers_;

  
  
  ManifestHandlerPriorityMap priority_map_;

  bool is_finalized_;
};

}  

#endif  
