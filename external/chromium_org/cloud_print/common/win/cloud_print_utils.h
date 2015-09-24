// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CLOUD_PRINT_COMMON_CLOUD_PRINT_UTILS_H_
#define CLOUD_PRINT_COMMON_CLOUD_PRINT_UTILS_H_

#include <wtypes.h>

#include "base/strings/string16.h"

namespace cloud_print {

HRESULT GetLastHResult();

base::string16 GetErrorMessage(HRESULT hr);

base::string16 LoadLocalString(DWORD id);

void SetGoogleUpdateUsage(const base::string16& product_id);

}  

#endif  

