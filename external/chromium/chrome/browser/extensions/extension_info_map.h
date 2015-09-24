// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_INFO_MAP_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_INFO_MAP_H_
#pragma once

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "chrome/common/extensions/extension.h"
#include "chrome/common/extensions/extension_extent.h"
#include "googleurl/src/gurl.h"

class Extension;

class ExtensionInfoMap : public base::RefCountedThreadSafe<ExtensionInfoMap> {
 public:
  ExtensionInfoMap();
  ~ExtensionInfoMap();

  
  void AddExtension(const Extension* extension);

  
  void RemoveExtension(const std::string& id,
                       const UnloadedExtensionInfo::Reason reason);

  
  std::string GetNameForExtension(const std::string& id) const;

  
  FilePath GetPathForExtension(const std::string& id) const;

  
  FilePath GetPathForDisabledExtension(const std::string& id) const;

  
  
  bool ExtensionHasWebExtent(const std::string& id) const;

  
  
  bool ExtensionCanLoadInIncognito(const std::string& id) const;

  
  
  std::string GetDefaultLocaleForExtension(const std::string& id) const;

  
  ExtensionExtent
      GetEffectiveHostPermissionsForExtension(const std::string& id) const;

  
  bool CheckURLAccessToExtensionPermission(const GURL& url,
                                           const char* permission_name) const;

  
  bool URLIsForExtensionIcon(const GURL& url) const;

 private:
  
  typedef std::map<std::string, scoped_refptr<const Extension> > Map;

  Map extension_info_;
  Map disabled_extension_info_;
};

#endif  
