// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_CAPI_UTIL_H_
#define CRYPTO_CAPI_UTIL_H_
#pragma once

#include <windows.h>
#include <wincrypt.h>

namespace crypto {

BOOL CryptAcquireContextLocked(HCRYPTPROV* prov,
                               LPCWSTR container,
                               LPCWSTR provider,
                               DWORD prov_type,
                               DWORD flags);

}  

#endif  
