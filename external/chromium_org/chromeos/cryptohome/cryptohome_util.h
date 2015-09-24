// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_CRYPTOHOME_CRYPTOHOME_UTIL_H_
#define CHROMEOS_CRYPTOHOME_CRYPTOHOME_UTIL_H_

#include <string>

#include "chromeos/chromeos_export.h"

namespace chromeos {

namespace cryptohome_util {

CHROMEOS_EXPORT bool TpmIsEnabled();

CHROMEOS_EXPORT bool TpmIsOwned();

CHROMEOS_EXPORT bool TpmIsBeingOwned();

CHROMEOS_EXPORT bool InstallAttributesGet(const std::string& name,
                                          std::string* value);
CHROMEOS_EXPORT bool InstallAttributesSet(const std::string& name,
                                          const std::string& value);
CHROMEOS_EXPORT bool InstallAttributesFinalize();
CHROMEOS_EXPORT bool InstallAttributesIsInvalid();
CHROMEOS_EXPORT bool InstallAttributesIsFirstInstall();

}  
}  

#endif  
