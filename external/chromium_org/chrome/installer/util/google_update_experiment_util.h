// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_GOOGLE_UPDATE_EXPERIMENT_UTIL_H_
#define CHROME_INSTALLER_UTIL_GOOGLE_UPDATE_EXPERIMENT_UTIL_H_

#include "base/strings/string16.h"

namespace base {
class Time;
}

namespace google_update {

#if defined(OS_WIN)
extern const wchar_t kExperimentLabels[];
#endif

extern const char kExperimentLabelSep[];

}  

namespace installer {

string16 BuildExperimentDateString(const base::Time& current_time);

}  

#endif  
