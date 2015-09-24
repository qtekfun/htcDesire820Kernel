// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_SYNC_APP_HELPER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_SYNC_APP_HELPER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/singleton.h"
#include "sync/api/string_ordinal.h"

class Profile;
class SyncTest;

class SyncAppHelper {
 public:
  
  static SyncAppHelper* GetInstance();

  
  
  void SetupIfNecessary(SyncTest* test);

  
  
  bool AppStatesMatch(Profile* profile1, Profile* profile2);

  
  syncer::StringOrdinal GetPageOrdinalForApp(Profile* profile,
                                             const std::string& name);

  
  
  void SetPageOrdinalForApp(Profile* profile,
                            const std::string& name,
                            const syncer::StringOrdinal& page_ordinal);

  
  
  syncer::StringOrdinal GetAppLaunchOrdinalForApp(Profile* profile,
                                                  const std::string& name);

  
  
  void SetAppLaunchOrdinalForApp(
      Profile* profile,
      const std::string& name,
      const syncer::StringOrdinal& app_launch_ordinal);

  
  void FixNTPOrdinalCollisions(Profile* profile);

 private:
  friend struct DefaultSingletonTraits<SyncAppHelper>;

  SyncAppHelper();
  ~SyncAppHelper();

  bool setup_completed_;

  DISALLOW_COPY_AND_ASSIGN(SyncAppHelper);
};

#endif  
