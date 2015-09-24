// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_ATTESTATION_FAKE_CERTIFICATE_H_
#define CHROME_BROWSER_CHROMEOS_ATTESTATION_FAKE_CERTIFICATE_H_

#include <string>

#include "base/time/time.h"

namespace chromeos {
namespace attestation {

bool GetFakeCertificate(const base::TimeDelta& expiry,
                        std::string* certificate);

}  
}  

#endif  
