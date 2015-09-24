// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_ATTESTATION_ATTESTATION_CONSTANTS_H_
#define CHROMEOS_ATTESTATION_ATTESTATION_CONSTANTS_H_

#include "chromeos/chromeos_export.h"

namespace chromeos {
namespace attestation {

enum AttestationKeyType {
  
  
  KEY_DEVICE,
  
  
  KEY_USER,
};

enum AttestationChallengeOptions {
  CHALLENGE_OPTION_NONE = 0,
  
  
  CHALLENGE_INCLUDE_SIGNED_PUBLIC_KEY = 1,
};

enum AttestationCertificateProfile {
  
  
  
  PROFILE_ENTERPRISE_MACHINE_CERTIFICATE,
  
  
  PROFILE_ENTERPRISE_USER_CERTIFICATE,
  
  PROFILE_CONTENT_PROTECTION_CERTIFICATE,
};

CHROMEOS_EXPORT extern const char kEnterpriseMachineKey[];

CHROMEOS_EXPORT extern const char kEnterpriseUserKey[];

CHROMEOS_EXPORT extern const char kContentProtectionKeyPrefix[];

}  
}  

#endif  
