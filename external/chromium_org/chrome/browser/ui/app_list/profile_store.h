// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_PROFILE_STORE_H_
#define CHROME_BROWSER_UI_APP_LIST_PROFILE_STORE_H_

#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "chrome/browser/profiles/profile_info_cache_observer.h"

class Profile;

class ProfileStore {
 public:
  virtual ~ProfileStore() {}
  virtual void AddProfileObserver(ProfileInfoCacheObserver* observer) = 0;

  
  
  virtual void LoadProfileAsync(const base::FilePath& path,
                                base::Callback<void(Profile*)> callback) = 0;

  
  virtual Profile* GetProfileByPath(const base::FilePath& path) = 0;

  
  
  virtual base::FilePath GetUserDataDir() = 0;

  
  virtual bool IsProfileManaged(const base::FilePath& path) = 0;
};

#endif  
