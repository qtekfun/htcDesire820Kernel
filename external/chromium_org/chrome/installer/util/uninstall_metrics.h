// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_UNINSTALL_METRICS_H_
#define CHROME_INSTALLER_UTIL_UNINSTALL_METRICS_H_

#include "base/strings/string16.h"

namespace base {
class DictionaryValue;
class FilePath;
}

namespace installer {

bool ExtractUninstallMetrics(const base::DictionaryValue& root,
                             string16* uninstall_metrics);

bool ExtractUninstallMetricsFromFile(const base::FilePath& file_path,
                                     string16* uninstall_metrics);

}  

#endif  
