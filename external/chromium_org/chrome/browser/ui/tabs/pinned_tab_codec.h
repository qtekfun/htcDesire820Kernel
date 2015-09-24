// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TABS_PINNED_TAB_CODEC_H_
#define CHROME_BROWSER_UI_TABS_PINNED_TAB_CODEC_H_

#include <vector>

#include "chrome/browser/ui/startup/startup_tab.h"
#include "url/gurl.h"

class Profile;

namespace base {
class Value;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

// preferences. When Chrome exits the sets of pinned tabs are written to prefs.
class PinnedTabCodec {
 public:
  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  static void WritePinnedTabs(Profile* profile);

  
  static void WritePinnedTabs(Profile* profile, const StartupTabs& tabs);

  
  static StartupTabs ReadPinnedTabs(Profile* profile);
  static StartupTabs ReadPinnedTabs(const base::Value* value);

 private:
  PinnedTabCodec();
  ~PinnedTabCodec();

  DISALLOW_COPY_AND_ASSIGN(PinnedTabCodec);
};

#endif  
