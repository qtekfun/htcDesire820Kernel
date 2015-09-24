// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INSTALL_VERIFICATION_WIN_MODULE_IDS_H_
#define CHROME_BROWSER_INSTALL_VERIFICATION_WIN_MODULE_IDS_H_

#include <map>
#include <string>

#include "base/strings/string_piece.h"

typedef std::map<std::string, size_t> ModuleIDs;

void ParseAdditionalModuleIDs(
    const base::StringPiece& raw_data,
    ModuleIDs* module_ids);

void LoadModuleIDs(ModuleIDs* module_ids);

#endif  
