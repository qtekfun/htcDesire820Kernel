// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_IMPORTER_SAFARI_IMPORTER_UTILS_H_
#define CHROME_COMMON_IMPORTER_SAFARI_IMPORTER_UTILS_H_

#include "base/basictypes.h"

namespace base {
class FilePath;
}

bool SafariImporterCanImport(const base::FilePath& library_dir,
                             uint16* services_supported);

#endif  
