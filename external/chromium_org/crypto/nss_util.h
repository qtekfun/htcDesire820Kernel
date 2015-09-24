// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_NSS_UTIL_H_
#define CRYPTO_NSS_UTIL_H_

#include <string>
#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "crypto/crypto_export.h"

namespace base {
class FilePath;
class Lock;
class Time;
}  

namespace crypto {

CRYPTO_EXPORT extern const char kTestTPMTokenName[];

#if defined(USE_NSS)
CRYPTO_EXPORT void EarlySetupForNSSInit();
#endif

CRYPTO_EXPORT void EnsureNSPRInit();

CRYPTO_EXPORT void InitNSSSafely();

CRYPTO_EXPORT void EnsureNSSInit();

CRYPTO_EXPORT void ForceNSSNoDBInit();

CRYPTO_EXPORT void DisableNSSForkCheck();

CRYPTO_EXPORT void LoadNSSLibraries();

bool CheckNSSVersion(const char* version);

#if defined(OS_CHROMEOS)
CRYPTO_EXPORT void OpenPersistentNSSDB();

CRYPTO_EXPORT void EnableTPMTokenForNSS();

CRYPTO_EXPORT bool IsTPMTokenEnabledForNSS();

CRYPTO_EXPORT bool IsTPMTokenReady(const base::Closure& callback)
    WARN_UNUSED_RESULT;

CRYPTO_EXPORT bool InitializeTPMToken(int token_slot_id);
#endif

CRYPTO_EXPORT base::Time PRTimeToBaseTime(int64 prtime);

CRYPTO_EXPORT int64 BaseTimeToPRTime(base::Time time);

#if defined(USE_NSS)
class CRYPTO_EXPORT_PRIVATE ScopedTestNSSDB {
 public:
  ScopedTestNSSDB();
  ~ScopedTestNSSDB();

  bool is_open() { return is_open_; }

 private:
  bool is_open_;
  DISALLOW_COPY_AND_ASSIGN(ScopedTestNSSDB);
};

base::Lock* GetNSSWriteLock();

class CRYPTO_EXPORT AutoNSSWriteLock {
 public:
  AutoNSSWriteLock();
  ~AutoNSSWriteLock();
 private:
  base::Lock *lock_;
  DISALLOW_COPY_AND_ASSIGN(AutoNSSWriteLock);
};

#endif  

}  

#endif  
