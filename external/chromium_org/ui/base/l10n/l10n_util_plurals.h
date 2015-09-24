// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef UI_BASE_L10N_L10N_UTIL_PLURALS_H_
#define UI_BASE_L10N_L10N_UTIL_PLURALS_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "third_party/icu/source/i18n/unicode/plurfmt.h"
#include "third_party/icu/source/i18n/unicode/plurrule.h"

namespace l10n_util {

scoped_ptr<icu::PluralRules> BuildPluralRules();

scoped_ptr<icu::PluralFormat> BuildPluralFormat(
    const std::vector<int>& message_ids);

}  

#endif  
