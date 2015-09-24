// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_IMPORTER_IE_IMPORTER_UTILS_WIN_H_
#define CHROME_COMMON_IMPORTER_IE_IMPORTER_UTILS_WIN_H_

#include "base/strings/string16.h"

namespace importer {

base::string16 GetIEFavoritesOrderKey();

base::string16 GetIE7PasswordsKey();

base::string16 GetIESettingsKey();

}  

#endif  
