// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_PENDING_EXTENSION_MANAGER_H_
#define EXTENSIONS_BROWSER_PENDING_EXTENSION_MANAGER_H_

#include <list>
#include <string>

#include "extensions/browser/pending_extension_info.h"
#include "extensions/common/manifest.h"

class ExtensionServiceInterface;
class GURL;

namespace base {
class Version;
}

FORWARD_DECLARE_TEST(ExtensionServiceTest,
                     UpdatePendingExtensionAlreadyInstalled);

namespace extensions {
class Extension;
class PendingExtensionManager;

class ExtensionUpdaterTest;
void SetupPendingExtensionManagerForTest(
    int count, const GURL& update_url,
    PendingExtensionManager* pending_extension_manager);

class PendingExtensionManager {
 public:
  
  
  
  
  explicit PendingExtensionManager(const ExtensionServiceInterface& service);
  ~PendingExtensionManager();

  
  

  
  
  bool Remove(const std::string& id);

  
  
  const PendingExtensionInfo* GetById(const std::string& id) const;

  
  bool IsIdPending(const std::string& id) const;

  
  bool HasPendingExtensions() const;

  
  bool HasPendingExtensionFromSync() const;

  
  
  
  
  
  
  
  
  
  bool AddFromSync(
      const std::string& id,
      const GURL& update_url,
      PendingExtensionInfo::ShouldAllowInstallPredicate should_allow_install,
      bool install_silently);

  
  bool AddFromExtensionImport(
      const std::string& id,
      const GURL& update_url,
      PendingExtensionInfo::ShouldAllowInstallPredicate should_allow_install);

  
  
  bool AddFromExternalUpdateUrl(const std::string& id,
                                const GURL& update_url,
                                Manifest::Location location,
                                int creation_flags,
                                bool mark_acknowledged);

  
  
  
  bool AddFromExternalFile(
      const std::string& id,
      Manifest::Location location,
      const base::Version& version,
      int creation_flags,
      bool mark_acknowledged);

  
  
  
  void GetPendingIdsForUpdateCheck(
      std::list<std::string>* out_ids_for_update_check) const;

 private:
  typedef std::list<PendingExtensionInfo> PendingExtensionList;

  
  
  bool AddExtensionImpl(
      const std::string& id,
      const GURL& update_url,
      const base::Version& version,
      PendingExtensionInfo::ShouldAllowInstallPredicate should_allow_install,
      bool is_from_sync,
      bool install_silently,
      Manifest::Location install_source,
      int creation_flags,
      bool mark_acknowledged);

  
  
  
  void AddForTesting(const PendingExtensionInfo& pending_extension_info);

  
  
  
  
  const ExtensionServiceInterface& service_;

  PendingExtensionList pending_extension_list_;

  FRIEND_TEST_ALL_PREFIXES(::ExtensionServiceTest,
                           UpdatePendingExtensionAlreadyInstalled);
  friend class ExtensionUpdaterTest;
  friend void SetupPendingExtensionManagerForTest(
      int count, const GURL& update_url,
      PendingExtensionManager* pending_extension_manager);

  DISALLOW_COPY_AND_ASSIGN(PendingExtensionManager);
};

}  

#endif  
