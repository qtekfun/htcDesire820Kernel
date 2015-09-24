// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_PROFILES_PROFILE_MANAGER_H_
#define CHROME_BROWSER_PROFILES_PROFILE_MANAGER_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/hash_tables.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/profiles/profile.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "ui/base/system_monitor/system_monitor.h"

class FilePath;

class ProfileManager : public base::NonThreadSafe,
                       public ui::SystemMonitor::PowerObserver,
                       public NotificationObserver,
                       public Profile::Delegate {
 public:
  class Observer {
   public:
    
    
    virtual void OnProfileCreated(Profile* profile) = 0;
  };

  ProfileManager();
  virtual ~ProfileManager();

  
  static void ShutdownSessionServices();

  
  
  
  
  Profile* GetDefaultProfile(const FilePath& user_data_dir);

  
  static Profile* GetDefaultProfile();

  
  
  
  Profile* GetProfile(const FilePath& profile_dir);

  
  
  
  void CreateProfileAsync(const FilePath& user_data_dir,
                          Observer* observer);

  
  
  static void CreateDefaultProfileAsync(Observer* observer);

  
  
  Profile* GetProfileWithId(ProfileId profile_id);

  
  
  bool IsValidProfile(Profile* profile);

  
  
  FilePath GetCurrentProfileDir();

  
  
  std::vector<Profile*> GetLoadedProfiles() const;

  
  virtual void OnSuspend();
  virtual void OnResume();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  

  
  
  static FilePath GetDefaultProfileDir(const FilePath& user_data_dir);

  
  static FilePath GetProfilePrefsPath(const FilePath& profile_dir);

  
  
  
  Profile* GetProfileByPath(const FilePath& path) const;

  
  virtual void OnProfileCreated(Profile* profile, bool success);

 protected:
  
  virtual void DoFinalInit(Profile* profile);

 private:
  friend class ExtensionEventRouterForwarderTest;

  
  
  struct ProfileInfo {
    ProfileInfo(Profile* profile, bool created)
        : profile(profile), created(created) {
    }

    ~ProfileInfo() {}

    scoped_ptr<Profile> profile;
    
    bool created;
    
    
    std::vector<Observer*> observers;

   private:
    DISALLOW_COPY_AND_ASSIGN(ProfileInfo);
  };

  
  
  
  
  bool AddProfile(Profile* profile);

  
  
  ProfileInfo* RegisterProfile(Profile* profile, bool created);

  NotificationRegistrar registrar_;

  
  
  
  bool logged_in_;

  
  
  typedef std::map<FilePath, linked_ptr<ProfileInfo> > ProfilesInfoMap;
  ProfilesInfoMap profiles_info_;

  DISALLOW_COPY_AND_ASSIGN(ProfileManager);
};

class ProfileManagerWithoutInit : public ProfileManager {
 protected:
  virtual void DoFinalInit(Profile*) {}
};

#endif  
