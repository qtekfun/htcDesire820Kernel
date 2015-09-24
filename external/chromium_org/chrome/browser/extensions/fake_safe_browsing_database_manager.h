// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_FAKE_SAFE_BROWSING_DATABASE_MANAGER_H_
#define CHROME_BROWSER_EXTENSIONS_FAKE_SAFE_BROWSING_DATABASE_MANAGER_H_

#include <set>
#include <string>

#include "chrome/browser/safe_browsing/database_manager.h"

namespace extensions {

class FakeSafeBrowsingDatabaseManager : public SafeBrowsingDatabaseManager {
 public:
  explicit FakeSafeBrowsingDatabaseManager(bool enabled);

  
  
  
  virtual bool CheckExtensionIDs(const std::set<std::string>& extension_ids,
                                 Client* client) OVERRIDE;

  
  FakeSafeBrowsingDatabaseManager& Enable();
  FakeSafeBrowsingDatabaseManager& ClearUnsafe();
  FakeSafeBrowsingDatabaseManager& SetUnsafe(const std::string& a);
  FakeSafeBrowsingDatabaseManager& SetUnsafe(const std::string& a,
                                             const std::string& b);
  FakeSafeBrowsingDatabaseManager& SetUnsafe(const std::string& a,
                                             const std::string& b,
                                             const std::string& c);

  
  void NotifyUpdate();

 private:
  virtual ~FakeSafeBrowsingDatabaseManager();

  
  void OnSafeBrowsingResult(scoped_ptr<SafeBrowsingCheck> result,
                            Client* client);

  
  
  bool enabled_;

  
  std::set<std::string> unsafe_ids_;
};

}  

#endif  
