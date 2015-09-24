// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_DIALOG_COMMON_H_
#define CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_DIALOG_COMMON_H_

#include "chrome/browser/ui/autofill/autofill_dialog_types.h"
#include "components/autofill/core/browser/autofill_type.h"
#include "components/autofill/core/browser/field_types.h"

namespace autofill {
class AutofillProfile;
}

namespace wallet {
class Address;
}

namespace autofill {
namespace common {

bool InputTypeMatchesFieldType(const DetailInput& input,
                               const AutofillType& field_type);

bool DetailInputMatchesField(DialogSection section,
                             const DetailInput& input,
                             const AutofillField& field);

bool IsCreditCardType(ServerFieldType type);

void BuildInputsForSection(DialogSection dialog_section, DetailInputs* inputs);

AutofillMetrics::DialogUiEvent DialogSectionToUiItemAddedEvent(
    DialogSection section);

AutofillMetrics::DialogUiEvent DialogSectionToUiSelectionChangedEvent(
    DialogSection section);

base::string16 GetHardcodedValueForType(ServerFieldType type);

}  
}  

#endif  
