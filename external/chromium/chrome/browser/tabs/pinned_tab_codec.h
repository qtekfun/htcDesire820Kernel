// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TABS_PINNED_TAB_CODEC_H_
#define CHROME_BROWSER_TABS_PINNED_TAB_CODEC_H_
#pragma once

#include <vector>

#include "chrome/browser/ui/browser_init.h"
#include "googleurl/src/gurl.h"

class PrefService;
class Profile;

// preferences. When Chrome exits the sets of pinned tabs are written to prefs.
class PinnedTabCodec {
 public:
  
  static void RegisterUserPrefs(PrefService* prefs);

  
  static void WritePinnedTabs(Profile* profile);

  
  static std::vector<BrowserInit::LaunchWithProfile::Tab> ReadPinnedTabs(
      Profile* profile);

 private:
  PinnedTabCodec();
  ~PinnedTabCodec();

  DISALLOW_COPY_AND_ASSIGN(PinnedTabCodec);
};

#endif  
