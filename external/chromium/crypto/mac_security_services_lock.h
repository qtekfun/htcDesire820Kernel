// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_MAC_SECURITY_SERVICES_LOCK_H_
#define CRYPTO_MAC_SECURITY_SERVICES_LOCK_H_
#pragma once

namespace base {
class Lock;
}


namespace crypto {

base::Lock& GetMacSecurityServicesLock();

}  

#endif  
