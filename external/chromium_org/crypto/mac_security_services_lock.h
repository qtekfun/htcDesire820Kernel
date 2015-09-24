// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_MAC_SECURITY_SERVICES_LOCK_H_
#define CRYPTO_MAC_SECURITY_SERVICES_LOCK_H_

#include "crypto/crypto_export.h"

namespace base {
class Lock;
}

namespace crypto {

CRYPTO_EXPORT base::Lock& GetMacSecurityServicesLock();

}  

#endif  
