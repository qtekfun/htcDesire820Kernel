// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_OMNIBOX_LOCATION_BAR_UTIL_H_
#define CHROME_BROWSER_UI_OMNIBOX_LOCATION_BAR_UTIL_H_
#pragma once

#include <string>

class Profile;

namespace location_bar_util {

std::wstring GetKeywordName(Profile* profile, const std::wstring& keyword);

std::wstring CalculateMinString(const std::wstring& description);

}  

#endif  
