// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILE_WINDOW_H_
#define CHROME_BROWSER_PROFILES_PROFILE_WINDOW_H_

#include "base/callback_forward.h"
#include "chrome/browser/ui/host_desktop.h"
#include "chrome/browser/ui/startup/startup_types.h"

class Profile;
namespace base { class FilePath; }

namespace profiles {

typedef base::Callback<void()> ProfileSwitchingDoneCallback;

void FindOrCreateNewWindowForProfile(
    Profile* profile,
    chrome::startup::IsProcessStartup process_startup,
    chrome::startup::IsFirstRun is_first_run,
    chrome::HostDesktopType desktop_type,
    bool always_create);


void SwitchToProfile(
    const base::FilePath& path,
    chrome::HostDesktopType desktop_type,
    bool always_create,
    ProfileSwitchingDoneCallback callback);

void SwitchToGuestProfile(chrome::HostDesktopType desktop_type,
                          ProfileSwitchingDoneCallback callback);

void CreateAndSwitchToNewProfile(chrome::HostDesktopType desktop_type,
                                 ProfileSwitchingDoneCallback callback);

void CloseGuestProfileWindows();

void LockProfile(Profile* profile);

}  

#endif  
