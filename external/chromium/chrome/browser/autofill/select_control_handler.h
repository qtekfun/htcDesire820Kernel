// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_SELECT_CONTROL_HANDLER_H_
#define CHROME_BROWSER_AUTOFILL_SELECT_CONTROL_HANDLER_H_
#pragma once

#include "chrome/browser/autofill/field_types.h"
#include "base/string16.h"

class FormGroup;

namespace webkit_glue {
struct FormField;
}  

namespace autofill {

void FillSelectControl(const FormGroup& form_group,
                       AutofillFieldType type,
                       webkit_glue::FormField* field);

bool IsValidState(const string16& value);

}  

#endif  
