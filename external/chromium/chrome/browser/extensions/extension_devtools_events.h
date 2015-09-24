// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_DEVTOOLS_EVENTS_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_DEVTOOLS_EVENTS_H_
#pragma once

#include <string>

#include "base/basictypes.h"

class ExtensionDevToolsEvents {
 public:
  
  
  static bool IsDevToolsEventName(const std::string& event_name, int* tab_id);

  
  static std::string OnPageEventNameForTab(int tab_id);

  
  static std::string OnTabCloseEventNameForTab(int tab_id);

 private:

  DISALLOW_IMPLICIT_CONSTRUCTORS(ExtensionDevToolsEvents);
};

#endif  

