// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_PUSH_MESSAGING_SYNC_SETUP_HELPER_H_
#define CHROME_BROWSER_EXTENSIONS_API_PUSH_MESSAGING_SYNC_SETUP_HELPER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "net/dns/mock_host_resolver.h"

class Profile;
class ProfileSyncServiceHarness;

namespace extensions {

class SyncSetupHelper {
 public:
  SyncSetupHelper();

  ~SyncSetupHelper();

  
  
  bool InitializeSync(Profile* profile);

  
  
  
  bool ReadPasswordFile(const base::FilePath& passwordFile);

  const std::string& client_id() const { return client_id_; }
  const std::string& client_secret() const { return client_secret_; }
  const std::string& refresh_token() const { return refresh_token_; }

 private:
  
  
  bool AwaitQuiescence();

  
  std::string username_;

  
  std::string password_;

  
  std::string client_id_;

  
  std::string client_secret_;

  
  std::string refresh_token_;

  
  
  Profile* profile_;

  
  
  
  scoped_ptr<ProfileSyncServiceHarness> client_;

  
  
  
  scoped_ptr<net::ScopedDefaultHostResolverProc> mock_host_resolver_override_;

  DISALLOW_COPY_AND_ASSIGN(SyncSetupHelper);
};

}  

#endif  
