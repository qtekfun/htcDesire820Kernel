// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_HELP_HELP_UTILS_CHROMEOS_H_
#define CHROME_BROWSER_UI_WEBUI_HELP_HELP_UTILS_CHROMEOS_H_

#include <string>

#include "base/strings/string16.h"

namespace help_utils_chromeos {

bool IsUpdateOverCellularAllowed();

base::string16 GetConnectionTypeAsUTF16(const std::string& type);

}  

#endif  
