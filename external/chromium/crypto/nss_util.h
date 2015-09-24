// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_NSS_UTIL_H_
#define CRYPTO_NSS_UTIL_H_
#pragma once

#include <string>
#include "base/basictypes.h"

#if defined(USE_NSS)
class FilePath;
#endif  

namespace base {
class Lock;
class Time;
}  

namespace crypto {

#if defined(USE_NSS)
void EarlySetupForNSSInit();
#endif

void EnsureNSPRInit();

void EnsureNSSInit();

void ForceNSSNoDBInit();

void DisableNSSForkCheck();

void LoadNSSLibraries();

bool CheckNSSVersion(const char* version);

#if defined(OS_CHROMEOS)
void OpenPersistentNSSDB();

class TPMTokenInfoDelegate {
 public:
  TPMTokenInfoDelegate();
  virtual ~TPMTokenInfoDelegate();
  virtual bool IsTokenReady() const = 0;
  virtual void GetTokenInfo(std::string* token_name,
                            std::string* user_pin) const = 0;
};

void EnableTPMTokenForNSS(TPMTokenInfoDelegate* delegate);

void GetTPMTokenInfo(std::string* token_name, std::string* user_pin);

bool IsTPMTokenReady();
#endif

base::Time PRTimeToBaseTime(int64 prtime);

#if defined(USE_NSS)
bool OpenTestNSSDB(const FilePath& path, const char* description);
void CloseTestNSSDB();

base::Lock* GetNSSWriteLock();

class AutoNSSWriteLock {
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
