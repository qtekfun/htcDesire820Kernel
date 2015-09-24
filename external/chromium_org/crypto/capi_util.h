// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_CAPI_UTIL_H_
#define CRYPTO_CAPI_UTIL_H_

#include <windows.h>
#include <wincrypt.h>

#include "crypto/crypto_export.h"

namespace crypto {

CRYPTO_EXPORT BOOL CryptAcquireContextLocked(HCRYPTPROV* prov,
                                             LPCWSTR container,
                                             LPCWSTR provider,
                                             DWORD prov_type,
                                             DWORD flags);

CRYPTO_EXPORT void* WINAPI CryptAlloc(size_t size);
CRYPTO_EXPORT void WINAPI CryptFree(void* p);

}  

#endif  
