// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_OPTIONS_OPTIONS_WINDOW_H_
#define CHROME_BROWSER_UI_OPTIONS_OPTIONS_WINDOW_H_
#pragma once

class Profile;

enum OptionsPage {
  OPTIONS_PAGE_DEFAULT = -1,
#if defined(OS_CHROMEOS)
  OPTIONS_PAGE_SYSTEM,
  OPTIONS_PAGE_INTERNET,
  OPTIONS_PAGE_ACCOUNTS,
#endif
  OPTIONS_PAGE_GENERAL,
  OPTIONS_PAGE_CONTENT,
  OPTIONS_PAGE_ADVANCED,
  OPTIONS_PAGE_COUNT
};

enum OptionsGroup {
  OPTIONS_GROUP_NONE,
  OPTIONS_GROUP_DEFAULT_SEARCH
};

#endif  
