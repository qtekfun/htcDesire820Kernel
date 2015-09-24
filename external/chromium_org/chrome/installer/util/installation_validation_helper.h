// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_INSTALLER_UTIL_INSTALLATION_VALIDATION_HELPER_H_
#define CHROME_INSTALLER_UTIL_INSTALLATION_VALIDATION_HELPER_H_

#include "chrome/installer/util/installation_validator.h"

namespace installer {

class InstallationState;

InstallationValidator::InstallationType ExpectValidInstallation(
    bool system_level);

InstallationValidator::InstallationType ExpectValidInstallationForState(
    const InstallationState& machine_state,
    bool system_level);

void ExpectInstallationOfType(
    bool system_level,
    InstallationValidator::InstallationType type);

void ExpectInstallationOfTypeForState(
    const InstallationState& machine_state,
    bool system_level,
    InstallationValidator::InstallationType type);

}  

#endif  
