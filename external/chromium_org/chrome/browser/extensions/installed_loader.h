// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_INSTALLED_LOADER_H_
#define CHROME_BROWSER_EXTENSIONS_INSTALLED_LOADER_H_

class ExtensionService;

namespace extensions {

class ExtensionPrefs;
struct ExtensionInfo;

class InstalledLoader {
 public:
  explicit InstalledLoader(ExtensionService* extension_service);
  virtual ~InstalledLoader();

  
  void Load(const ExtensionInfo& info, bool write_to_prefs);

  
  void LoadAllExtensions();

 private:
  
  
  int GetCreationFlags(const ExtensionInfo* info);

  ExtensionService* extension_service_;

  ExtensionPrefs* extension_prefs_;
};

}  

#endif  
