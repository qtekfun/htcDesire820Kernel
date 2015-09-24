// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_FILE_UTIL_ICU_H_
#define BASE_I18N_FILE_UTIL_ICU_H_


#include "base/files/file_path.h"
#include "base/i18n/base_i18n_export.h"
#include "base/strings/string16.h"

namespace file_util {

BASE_I18N_EXPORT bool IsFilenameLegal(const base::string16& file_name);

BASE_I18N_EXPORT void ReplaceIllegalCharactersInPath(
    base::FilePath::StringType* file_name,
    char replace_char);

BASE_I18N_EXPORT bool LocaleAwareCompareFilenames(const base::FilePath& a,
                                                  const base::FilePath& b);

BASE_I18N_EXPORT void NormalizeFileNameEncoding(base::FilePath* file_name);

}  

#endif  
