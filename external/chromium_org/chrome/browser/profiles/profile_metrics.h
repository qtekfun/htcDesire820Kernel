// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILE_METRICS_H_
#define CHROME_BROWSER_PROFILES_PROFILE_METRICS_H_

#include <stddef.h>
#include <string>

#include "base/basictypes.h"

class Profile;
class ProfileManager;

namespace base {
class FilePath;
}

class ProfileMetrics {
 public:
  
  enum ProfileAdd {
    ADD_NEW_USER_ICON = 0,    
    ADD_NEW_USER_MENU,        
    ADD_NEW_USER_DIALOG,      
    NUM_PROFILE_ADD_METRICS
  };

  
  enum ProfileOpen {
    NTP_AVATAR_BUBBLE = 0,    
    ICON_AVATAR_BUBBLE,       
    SWITCH_PROFILE_ICON,      
    SWITCH_PROFILE_MENU,      
    SWITCH_PROFILE_DOCK,      
    NUM_PROFILE_OPEN_METRICS
  };

  
  enum ProfileNetUserCounts {
    ADD_NEW_USER = 0,         
    PROFILE_DELETED,          
    NUM_PROFILE_NET_METRICS
  };

  
  
  
  enum ProfileSync {
    SYNC_CUSTOMIZE = 0,       
    SYNC_CHOOSE,              
    SYNC_ENCRYPT,             
    SYNC_PASSPHRASE,          
    NUM_PROFILE_SYNC_METRICS
  };

  enum ProfileType {
    ORIGINAL = 0,         
    SECONDARY,            
    NUM_PROFILE_TYPE_METRICS
  };

  enum ProfileGaia {
    GAIA_OPT_IN = 0,           
    GAIA_OPT_OUT,              
    NUM_PROFILE_GAIA_METRICS
  };

  static void UpdateReportedProfilesStatistics(ProfileManager* manager);

  static void LogNumberOfProfiles(ProfileManager* manager);
  static void LogProfileAddNewUser(ProfileAdd metric);
  static void LogProfileAvatarSelection(size_t icon_index);
  static void LogProfileDeleteUser(ProfileNetUserCounts metric);
  static void LogProfileOpenMethod(ProfileOpen metric);
  static void LogProfileSwitchGaia(ProfileGaia metric);
  static void LogProfileSwitchUser(ProfileOpen metric);
  static void LogProfileSyncInfo(ProfileSync metric);

  
  
  static void LogProfileLaunch(Profile* profile);
  static void LogProfileSyncSignIn(const base::FilePath& profile_path);
  static void LogProfileUpdate(const base::FilePath& profile_path);
};


#endif  
