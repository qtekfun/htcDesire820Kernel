// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_WIN_WIN_UTIL_H_
#define BASE_WIN_WIN_UTIL_H_

#include <windows.h>

#include <string>

#include "base/base_export.h"
#include "base/strings/string16.h"

struct IPropertyStore;
struct _tagpropertykey;
typedef _tagpropertykey PROPERTYKEY;

namespace base {
namespace win {

BASE_EXPORT void GetNonClientMetrics(NONCLIENTMETRICS* metrics);

BASE_EXPORT bool GetUserSidString(std::wstring* user_sid);

BASE_EXPORT bool IsShiftPressed();

BASE_EXPORT bool IsCtrlPressed();

BASE_EXPORT bool IsAltPressed();

BASE_EXPORT bool IsAltGrPressed();

BASE_EXPORT bool UserAccountControlIsEnabled();

BASE_EXPORT bool SetBooleanValueForPropertyStore(
    IPropertyStore* property_store,
    const PROPERTYKEY& property_key,
    bool property_bool_value);

BASE_EXPORT bool SetStringValueForPropertyStore(
    IPropertyStore* property_store,
    const PROPERTYKEY& property_key,
    const wchar_t* property_string_value);

BASE_EXPORT bool SetAppIdForPropertyStore(IPropertyStore* property_store,
                                          const wchar_t* app_id);

BASE_EXPORT bool AddCommandToAutoRun(HKEY root_key, const string16& name,
                                     const string16& command);
BASE_EXPORT bool RemoveCommandFromAutoRun(HKEY root_key, const string16& name);

BASE_EXPORT bool ReadCommandFromAutoRun(HKEY root_key,
                                        const string16& name,
                                        string16* command);

BASE_EXPORT void SetShouldCrashOnProcessDetach(bool crash);
BASE_EXPORT bool ShouldCrashOnProcessDetach();

BASE_EXPORT void SetAbortBehaviorForCrashReporting();

BASE_EXPORT bool IsTouchEnabledDevice();

#define SIZEOF_STRUCT_WITH_SPECIFIED_LAST_MEMBER(struct_name, member) \
    offsetof(struct_name, member) + \
    (sizeof static_cast<struct_name*>(NULL)->member)

BASE_EXPORT bool DisplayVirtualKeyboard();

BASE_EXPORT bool DismissVirtualKeyboard();

BASE_EXPORT BOOL GetMonitorInfoWrapper(HMONITOR monitor, MONITORINFO* mi);

BASE_EXPORT bool IsEnrolledToDomain();

}  
}  

#endif  
