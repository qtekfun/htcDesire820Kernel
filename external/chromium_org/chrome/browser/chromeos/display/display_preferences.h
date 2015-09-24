// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DISPLAY_DISPLAY_PREFERENCES_H_
#define CHROME_BROWSER_CHROMEOS_DISPLAY_DISPLAY_PREFERENCES_H_

#include "base/basictypes.h"
#include "third_party/cros_system_api/dbus/service_constants.h"

class PrefRegistrySimple;

namespace ash {
struct DisplayLayout;
}

namespace gfx {
class Display;
class Insets;
}

namespace chromeos {

void RegisterDisplayLocalStatePrefs(PrefRegistrySimple* registry);

void StoreDisplayPrefs();

void SetCurrentDisplayLayout(const ash::DisplayLayout& layout);

void LoadDisplayPreferences(bool first_run_after_boot);

void StoreDisplayLayoutPrefForTest(int64 id1,
                                   int64 id2,
                                   const ash::DisplayLayout& layout);

void StoreDisplayPowerStateForTest(DisplayPowerState power_state);

}  

#endif  
