// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_STARTUP_STARTUP_TAB_H_
#define CHROME_BROWSER_UI_STARTUP_STARTUP_TAB_H_

#include <string>
#include <vector>

#include "url/gurl.h"

struct StartupTab {
  StartupTab();
  ~StartupTab();

  
  GURL url;

  
  
  bool is_app;

  
  bool is_pinned;

  
  std::string app_id;
};

typedef std::vector<StartupTab> StartupTabs;

#endif  
