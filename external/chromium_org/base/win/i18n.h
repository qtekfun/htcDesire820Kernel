// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_WIN_I18N_H_
#define BASE_WIN_I18N_H_

#include <string>
#include <vector>

#include "base/base_export.h"
#include "base/basictypes.h"

namespace base {
namespace win {
namespace i18n {

BASE_EXPORT bool GetUserPreferredUILanguageList(
    std::vector<std::wstring>* languages);

BASE_EXPORT bool GetThreadPreferredUILanguageList(
    std::vector<std::wstring>* languages);

}  
}  
}  

#endif  
