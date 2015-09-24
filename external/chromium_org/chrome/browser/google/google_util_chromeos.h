// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GOOGLE_GOOGLE_UTIL_CHROMEOS_H_
#define CHROME_BROWSER_GOOGLE_GOOGLE_UTIL_CHROMEOS_H_

#include <string>

#include "base/callback_forward.h"

namespace google_util {
namespace chromeos {

std::string GetBrand();

void ClearBrandForCurrentSession();

void SetBrandFromFile(const base::Closure& callback);

}  
}  

#endif  
