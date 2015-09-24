// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_APP_LIST_SERVICE_IMPL_H_
#define CHROME_BROWSER_UI_APP_LIST_APP_LIST_SERVICE_IMPL_H_

#include <string>

#include "base/basictypes.h"
#include "base/command_line.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_info_cache_observer.h"
#include "chrome/browser/ui/app_list/app_list_service.h"
#include "chrome/browser/ui/app_list/keep_alive_service.h"
#include "chrome/browser/ui/app_list/profile_loader.h"

class ProfileStore;

namespace base {
class FilePath;
}

class AppListServiceImpl : public AppListService,
                           public ProfileInfoCacheObserver {
 public:
  static void RecordAppListLaunch();
  static void RecordAppListAppLaunch();
  virtual ~AppListServiceImpl();

  
  AppListServiceImpl(const CommandLine& command_line,
                     PrefService* local_state,
                     scoped_ptr<ProfileStore> profile_store,
                     scoped_ptr<KeepAliveService> keep_alive_service);

  
  virtual void SetAppListNextPaintCallback(void (*callback)()) OVERRIDE;
  virtual void HandleFirstRun() OVERRIDE;
  virtual void Init(Profile* initial_profile) OVERRIDE;
  virtual base::FilePath GetProfilePath(
      const base::FilePath& user_data_dir) OVERRIDE;
  virtual void SetProfilePath(const base::FilePath& profile_path) OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void EnableAppList(Profile* initial_profile) OVERRIDE;

 protected:
  AppListServiceImpl();

  void InvalidatePendingProfileLoads();
  ProfileLoader& profile_loader() { return *profile_loader_; }
  const ProfileLoader& profile_loader() const { return *profile_loader_; }

  
  
  void HandleCommandLineFlags(Profile* initial_profile);

  
  void SendUsageStats();

  
  virtual void CreateShortcut();

 private:
  static void SendAppListStats();

  
  void LoadProfileAsync(const base::FilePath& profile_file_path);

  
  void OnProfileLoaded(int profile_load_sequence_id,
                       Profile* profile,
                       Profile::CreateStatus status);

  
  virtual void OnProfileWillBeRemoved(
      const base::FilePath& profile_path) OVERRIDE;

  scoped_ptr<ProfileStore> profile_store_;
  base::WeakPtrFactory<AppListServiceImpl> weak_factory_;
  CommandLine command_line_;
  PrefService* local_state_;
  scoped_ptr<ProfileLoader> profile_loader_;

  DISALLOW_COPY_AND_ASSIGN(AppListServiceImpl);
};

#endif  
