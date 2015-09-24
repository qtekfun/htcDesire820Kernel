// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/strings/string16.h"

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_STATE_NAMES_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_STATE_NAMES_H_

namespace autofill {
namespace state_names {

base::string16 GetAbbreviationForName(const base::string16& name);

base::string16 GetNameForAbbreviation(const base::string16& abbreviation);

void GetNameAndAbbreviation(const base::string16& value,
                            base::string16* name,
                            base::string16* abbreviation);

}  
}  

#endif  
