// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_NUMBER_FORMATTING_H_
#define BASE_I18N_NUMBER_FORMATTING_H_

#include "base/basictypes.h"
#include "base/i18n/base_i18n_export.h"
#include "base/strings/string16.h"

namespace base {

BASE_I18N_EXPORT string16 FormatNumber(int64 number);

BASE_I18N_EXPORT string16 FormatDouble(double number, int fractional_digits);

namespace testing {

BASE_I18N_EXPORT void ResetFormatters();

}  

}  

#endif  
