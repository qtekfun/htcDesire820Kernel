// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_FILE_UTIL_ICU_H_
#define BASE_I18N_FILE_UTIL_ICU_H_
#pragma once


#include "base/file_path.h"
#include "base/string16.h"

namespace file_util {

bool IsFilenameLegal(const string16& file_name);

void ReplaceIllegalCharactersInPath(FilePath::StringType* file_name,
                                    char replace_char);

bool LocaleAwareCompareFilenames(const FilePath& a, const FilePath& b);

}  

#endif  
