// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_PENDING_EXTENSION_MANAGER_H_
#define CHROME_BROWSER_EXTENSIONS_PENDING_EXTENSION_MANAGER_H_
#pragma once

#include <map>
#include <string>

#include "chrome/browser/extensions/pending_extension_info.h"
#include "chrome/common/extensions/extension.h"

class ExtensionServiceInterface;
class GURL;

class PendingExtensionManager {
 public:
  
  
  typedef std::map<std::string, PendingExtensionInfo> PendingExtensionMap;
  typedef PendingExtensionMap::const_iterator const_iterator;

  
  
  
  
  explicit PendingExtensionManager(const ExtensionServiceInterface& service);
  ~PendingExtensionManager();

  
  

  
  void Remove(const std::string& id);

  
  
  
  bool GetById(const std::string& id,
               PendingExtensionInfo* out_pending_extension_info) const;

  
  bool IsIdPending(const std::string& id) const;

  
  const_iterator begin() const { return  pending_extension_map_.begin(); }
  const_iterator end() const { return  pending_extension_map_.end(); }

  
  
  
  
  
  
  
  
  
  bool AddFromSync(
      const std::string& id,
      const GURL& update_url,
      PendingExtensionInfo::ShouldAllowInstallPredicate should_allow_install,
      bool install_silently,
      bool enable_on_install,
      bool enable_incognito_on_install);

  
  
  void AddFromExternalUpdateUrl(const std::string& id,
                                const GURL& update_url,
                                Extension::Location location);

  
  void AddFromDefaultAppList(const std::string& id);

  
  void AddFromExternalFile(
      const std::string& id,
      Extension::Location location);

 private:
  
  
  bool AddExtensionImpl(
      const std::string& id,
      const GURL& update_url,
      PendingExtensionInfo::ShouldAllowInstallPredicate should_allow_install,
      bool is_from_sync,
      bool install_silently,
      bool enable_on_install,
      bool enable_incognito_on_install,
      Extension::Location install_source);

  
  
  
  void AddForTesting(const std::string& id,
                     const PendingExtensionInfo& pending_etension_info);

  
  
  
  
  const ExtensionServiceInterface& service_;

  
  PendingExtensionMap pending_extension_map_;

  FRIEND_TEST_ALL_PREFIXES(ExtensionServiceTest,
                           UpdatePendingExtensionAlreadyInstalled);
  friend class ExtensionUpdaterTest;
  friend void SetupPendingExtensionManagerForTest(
      int count, const GURL& update_url,
      PendingExtensionManager* pending_extension_manager);

  DISALLOW_COPY_AND_ASSIGN(PendingExtensionManager);
};

#endif  
