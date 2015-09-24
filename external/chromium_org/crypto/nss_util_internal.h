// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_NSS_UTIL_INTERNAL_H_
#define CRYPTO_NSS_UTIL_INTERNAL_H_

#include <secmodt.h>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "crypto/crypto_export.h"
#include "crypto/scoped_nss_types.h"

namespace base {
class FilePath;
}


namespace crypto {

CRYPTO_EXPORT PK11SlotInfo* GetPublicNSSKeySlot() WARN_UNUSED_RESULT;

CRYPTO_EXPORT PK11SlotInfo* GetPrivateNSSKeySlot() WARN_UNUSED_RESULT;

class AutoSECMODListReadLock {
 public:
  AutoSECMODListReadLock();
  ~AutoSECMODListReadLock();

 private:
  SECMODListLock* lock_;
  DISALLOW_COPY_AND_ASSIGN(AutoSECMODListReadLock);
};

#if defined(OS_CHROMEOS)
CRYPTO_EXPORT bool InitializeNSSForChromeOSUser(
    const std::string& email,
    const std::string& username_hash,
    bool is_primary_user,
    const base::FilePath& path) WARN_UNUSED_RESULT;

CRYPTO_EXPORT void InitializeTPMForChromeOSUser(
    const std::string& username_hash,
    CK_SLOT_ID slot_id);

CRYPTO_EXPORT void InitializePrivateSoftwareSlotForChromeOSUser(
    const std::string& username_hash);

CRYPTO_EXPORT ScopedPK11Slot GetPublicSlotForChromeOSUser(
    const std::string& username_hash) WARN_UNUSED_RESULT;

CRYPTO_EXPORT ScopedPK11Slot GetPrivateSlotForChromeOSUser(
    const std::string& username_hash,
    const base::Callback<void(ScopedPK11Slot)>& callback) WARN_UNUSED_RESULT;
#endif  

}  

#endif  
