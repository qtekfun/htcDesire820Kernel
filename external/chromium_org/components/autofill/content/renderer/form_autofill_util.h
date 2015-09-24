// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_RENDERER_FORM_AUTOFILL_UTIL_H_
#define COMPONENTS_AUTOFILL_CONTENT_RENDERER_FORM_AUTOFILL_UTIL_H_

#include <vector>

#include "base/strings/string16.h"

namespace blink {
class WebDocument;
class WebElement;
class WebFormElement;
class WebFormControlElement;
class WebFrame;
class WebInputElement;
class WebNode;
}

namespace autofill {

struct FormData;
struct FormFieldData;
struct WebElementDescriptor;

enum RequirementsMask {
  REQUIRE_NONE         = 0,  
  REQUIRE_AUTOCOMPLETE = 1,  
};

enum ExtractMask {
  EXTRACT_NONE        = 0,
  EXTRACT_VALUE       = 1 << 0,  
  EXTRACT_OPTION_TEXT = 1 << 1,  
                                 
                                 
                                 
                                 
  EXTRACT_OPTIONS     = 1 << 2,  
                                 
};

extern const size_t kMaxParseableFields;

bool IsMonthInput(const blink::WebInputElement* element);

bool IsTextInput(const blink::WebInputElement* element);

bool IsSelectElement(const blink::WebFormControlElement& element);

bool IsTextAreaElement(const blink::WebFormControlElement& element);

bool IsCheckableElement(const blink::WebInputElement* element);

bool IsAutofillableInputElement(const blink::WebInputElement* element);

bool IsWebNodeVisible(const blink::WebNode& node);

const base::string16 GetFormIdentifier(const blink::WebFormElement& form);

bool ClickElement(const blink::WebDocument& document,
                  const WebElementDescriptor& element_descriptor);

void ExtractAutofillableElements(
    const blink::WebFormElement& form_element,
    RequirementsMask requirements,
    std::vector<blink::WebFormControlElement>* autofillable_elements);

void WebFormControlElementToFormField(
    const blink::WebFormControlElement& element,
    ExtractMask extract_mask,
    FormFieldData* field);

bool WebFormElementToFormData(
    const blink::WebFormElement& form_element,
    const blink::WebFormControlElement& form_control_element,
    RequirementsMask requirements,
    ExtractMask extract_mask,
    FormData* form,
    FormFieldData* field);

bool FindFormAndFieldForInputElement(const blink::WebInputElement& element,
                                     FormData* form,
                                     FormFieldData* field,
                                     RequirementsMask requirements);

void FillForm(const FormData& form,
              const blink::WebInputElement& element);

void FillFormIncludingNonFocusableElements(
    const FormData& form_data,
    const blink::WebFormElement& form_element);

void FillFormForAllElements(
    const FormData& form_data,
    const blink::WebFormElement& form_element);

void PreviewForm(const FormData& form,
                 const blink::WebInputElement& element);

bool ClearPreviewedFormWithElement(const blink::WebInputElement& element,
                                   bool was_autofilled);

bool FormWithElementIsAutofilled(const blink::WebInputElement& element);

bool IsWebpageEmpty(const blink::WebFrame* frame);

bool IsWebElementEmpty(const blink::WebElement& element);

}  

#endif  
