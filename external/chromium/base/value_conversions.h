// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_VALUE_CONVERSIONS_H_
#define BASE_VALUE_CONVERSIONS_H_
#pragma once


#include "base/base_api.h"

class FilePath;
class StringValue;
class Value;

namespace base {

BASE_API StringValue* CreateFilePathValue(const FilePath& in_value);
BASE_API bool GetValueAsFilePath(const Value& value, FilePath* file_path);

}  

#endif  
