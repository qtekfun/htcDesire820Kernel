// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_WIN_WIN_UTIL_H_
#define BASE_WIN_WIN_UTIL_H_
#pragma once

#include <windows.h>

#include <string>

#include "base/base_api.h"
#include "base/string16.h"

struct IPropertyStore;
struct _tagpropertykey;
typedef _tagpropertykey PROPERTYKEY;

namespace base {
namespace win {

BASE_API void GetNonClientMetrics(NONCLIENTMETRICS* metrics);

BASE_API bool GetUserSidString(std::wstring* user_sid);

BASE_API bool IsShiftPressed();

BASE_API bool IsCtrlPressed();

BASE_API bool IsAltPressed();

BASE_API bool UserAccountControlIsEnabled();

BASE_API bool SetAppIdForPropertyStore(IPropertyStore* property_store,
                                       const wchar_t* app_id);

BASE_API bool AddCommandToAutoRun(HKEY root_key, const string16& name,
                                  const string16& command);
BASE_API bool RemoveCommandFromAutoRun(HKEY root_key, const string16& name);

BASE_API bool ReadCommandFromAutoRun(HKEY root_key,
                                     const string16& name,
                                     string16* command);
}  
}  

#endif  
