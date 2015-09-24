// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_CSSM_INIT_H_
#define CRYPTO_CSSM_INIT_H_
#pragma once

#include <Security/cssm.h>

#include "base/basictypes.h"

namespace crypto {

void EnsureCSSMInit();

CSSM_CSP_HANDLE GetSharedCSPHandle();

CSSM_CL_HANDLE GetSharedCLHandle();

CSSM_TP_HANDLE GetSharedTPHandle();

extern const CSSM_API_MEMORY_FUNCS kCssmMemoryFunctions;

void LogCSSMError(const char *function_name, CSSM_RETURN err);

void* CSSMMalloc(CSSM_SIZE size);
void CSSMFree(void* ptr);

class ScopedCSSMData {
 public:
  ScopedCSSMData();
  ~ScopedCSSMData();
  operator CSSM_DATA*() { return &data_; }
  CSSM_DATA* operator ->() { return &data_; }

 private:
  CSSM_DATA data_;

  DISALLOW_COPY_AND_ASSIGN(ScopedCSSMData);
};

}  

#endif  
