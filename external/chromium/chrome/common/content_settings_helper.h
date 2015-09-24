// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_CONTENT_SETTINGS_HELPER_H_
#define CHROME_COMMON_CONTENT_SETTINGS_HELPER_H_
#pragma once

#include <string>

#include "base/string16.h"

class GURL;

namespace content_settings_helper {

std::string OriginToString(const GURL& origin);
string16 OriginToString16(const GURL& origin);

}  

#endif  
