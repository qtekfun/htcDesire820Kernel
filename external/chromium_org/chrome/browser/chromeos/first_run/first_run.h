// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FIRST_RUN_FIRST_RUN_H_
#define CHROME_BROWSER_CHROMEOS_FIRST_RUN_FIRST_RUN_H_

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace chromeos {
namespace first_run {

void MaybeLaunchDialogAfterSessionStart();

void LaunchTutorial();

}  
}  

#endif  
