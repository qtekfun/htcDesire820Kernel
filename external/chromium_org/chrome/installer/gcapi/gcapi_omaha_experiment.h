// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_GCAPI_GCAPI_OMAHA_EXPERIMENT_H_
#define CHROME_INSTALLER_GCAPI_GCAPI_OMAHA_EXPERIMENT_H_

#include "base/strings/string16.h"

namespace gcapi_internals {

extern const wchar_t kReactivationLabel[];
extern const wchar_t kRelaunchLabel[];

string16 GetGCAPIExperimentLabel(const wchar_t* brand_code,
                                 const string16& label);

}  

// GCAPI_INVOKED_UAC_ELEVATION, the value will be written to HKLM, otherwise
bool SetReactivationExperimentLabels(const wchar_t* brand_code, int shell_mode);

// GCAPI_INVOKED_UAC_ELEVATION, the value will be written to HKLM, otherwise
bool SetRelaunchExperimentLabels(const wchar_t* brand_code, int shell_mode);

#endif  
