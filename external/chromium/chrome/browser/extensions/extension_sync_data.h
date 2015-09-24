// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_SYNC_DATA_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_SYNC_DATA_H_
#pragma once

#include <string>

#include "base/version.h"
#include "googleurl/src/gurl.h"

struct ExtensionSyncData {
  ExtensionSyncData();
  ~ExtensionSyncData();

  std::string id;

  
  
  
  bool uninstalled;
  bool enabled;
  bool incognito_enabled;

  
  
  Version version;
  GURL update_url;
};

#endif  
