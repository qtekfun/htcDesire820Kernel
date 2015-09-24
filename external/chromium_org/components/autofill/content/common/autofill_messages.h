// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <string>

#include "base/time/time.h"
#include "components/autofill/content/common/autofill_param_traits_macros.h"
#include "components/autofill/core/common/form_data.h"
#include "components/autofill/core/common/form_data_predictions.h"
#include "components/autofill/core/common/form_field_data.h"
#include "components/autofill/core/common/form_field_data_predictions.h"
#include "components/autofill/core/common/forms_seen_state.h"
#include "components/autofill/core/common/password_form.h"
#include "components/autofill/core/common/password_form_fill_data.h"
#include "components/autofill/core/common/web_element_descriptor.h"
#include "content/public/common/common_param_traits.h"
#include "content/public/common/common_param_traits_macros.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_message_utils.h"
#include "third_party/WebKit/public/web/WebFormElement.h"
#include "ui/gfx/rect.h"

#define IPC_MESSAGE_START AutofillMsgStart

IPC_ENUM_TRAITS_MAX_VALUE(autofill::FormsSeenState,
                          autofill::FORMS_SEEN_STATE_NUM_STATES - 1)
IPC_ENUM_TRAITS_MAX_VALUE(base::i18n::TextDirection,
                          base::i18n::TEXT_DIRECTION_NUM_DIRECTIONS - 1)

IPC_STRUCT_TRAITS_BEGIN(autofill::WebElementDescriptor)
  IPC_STRUCT_TRAITS_MEMBER(descriptor)
  IPC_STRUCT_TRAITS_MEMBER(retrieval_method)
IPC_STRUCT_TRAITS_END()

IPC_ENUM_TRAITS_MAX_VALUE(autofill::WebElementDescriptor::RetrievalMethod,
                          autofill::WebElementDescriptor::NONE)

IPC_STRUCT_TRAITS_BEGIN(autofill::FormFieldData)
  IPC_STRUCT_TRAITS_MEMBER(label)
  IPC_STRUCT_TRAITS_MEMBER(name)
  IPC_STRUCT_TRAITS_MEMBER(value)
  IPC_STRUCT_TRAITS_MEMBER(form_control_type)
  IPC_STRUCT_TRAITS_MEMBER(autocomplete_attribute)
  IPC_STRUCT_TRAITS_MEMBER(max_length)
  IPC_STRUCT_TRAITS_MEMBER(is_autofilled)
  IPC_STRUCT_TRAITS_MEMBER(is_checked)
  IPC_STRUCT_TRAITS_MEMBER(is_checkable)
  IPC_STRUCT_TRAITS_MEMBER(is_focusable)
  IPC_STRUCT_TRAITS_MEMBER(should_autocomplete)
  IPC_STRUCT_TRAITS_MEMBER(text_direction)
  IPC_STRUCT_TRAITS_MEMBER(option_values)
  IPC_STRUCT_TRAITS_MEMBER(option_contents)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(autofill::FormFieldDataPredictions)
  IPC_STRUCT_TRAITS_MEMBER(field)
  IPC_STRUCT_TRAITS_MEMBER(signature)
  IPC_STRUCT_TRAITS_MEMBER(heuristic_type)
  IPC_STRUCT_TRAITS_MEMBER(server_type)
  IPC_STRUCT_TRAITS_MEMBER(overall_type)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(autofill::FormDataPredictions)
  IPC_STRUCT_TRAITS_MEMBER(data)
  IPC_STRUCT_TRAITS_MEMBER(signature)
  IPC_STRUCT_TRAITS_MEMBER(experiment_id)
  IPC_STRUCT_TRAITS_MEMBER(fields)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(autofill::UsernamesCollectionKey)
  IPC_STRUCT_TRAITS_MEMBER(username)
  IPC_STRUCT_TRAITS_MEMBER(password)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(autofill::PasswordFormFillData)
  IPC_STRUCT_TRAITS_MEMBER(basic_data)
  IPC_STRUCT_TRAITS_MEMBER(preferred_realm)
  IPC_STRUCT_TRAITS_MEMBER(additional_logins)
  IPC_STRUCT_TRAITS_MEMBER(other_possible_usernames)
  IPC_STRUCT_TRAITS_MEMBER(wait_for_username)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(autofill::PasswordAndRealm)
  IPC_STRUCT_TRAITS_MEMBER(password)
  IPC_STRUCT_TRAITS_MEMBER(realm)
IPC_STRUCT_TRAITS_END()

IPC_ENUM_TRAITS_MAX_VALUE(
    blink::WebFormElement::AutocompleteResult,
    blink::WebFormElement::AutocompleteResultErrorInvalid)


IPC_MESSAGE_ROUTED2(AutofillMsg_FormDataFilled,
                    int ,
                    autofill::FormData )

IPC_MESSAGE_ROUTED1(AutofillMsg_FillPasswordForm,
                    autofill::PasswordFormFillData )

IPC_MESSAGE_ROUTED1(
    AutofillMsg_FieldTypePredictionsAvailable,
    std::vector<autofill::FormDataPredictions> )

IPC_MESSAGE_ROUTED0(AutofillMsg_SetAutofillActionFill)

IPC_MESSAGE_ROUTED0(AutofillMsg_ClearForm)

IPC_MESSAGE_ROUTED0(AutofillMsg_SetAutofillActionPreview)

IPC_MESSAGE_ROUTED0(AutofillMsg_ClearPreviewedForm)

IPC_MESSAGE_ROUTED1(AutofillMsg_SetNodeText,
                    base::string16 )

IPC_MESSAGE_ROUTED1(AutofillMsg_AcceptDataListSuggestion,
                    base::string16 )

IPC_MESSAGE_ROUTED1(AutofillMsg_GeneratedPasswordAccepted,
                    base::string16 )

IPC_MESSAGE_ROUTED1(AutofillMsg_AcceptPasswordAutofillSuggestion,
                    base::string16 )

IPC_MESSAGE_ROUTED1(AutofillMsg_FormNotBlacklisted,
                    autofill::PasswordForm )

IPC_MESSAGE_ROUTED2(AutofillMsg_RequestAutocompleteResult,
                    blink::WebFormElement::AutocompleteResult ,
                    autofill::FormData )

IPC_MESSAGE_ROUTED0(AutofillMsg_PageShown)

IPC_MESSAGE_ROUTED1(AutofillMsg_AccountCreationFormsDetected,
                    std::vector<autofill::FormData> )



IPC_MESSAGE_ROUTED3(AutofillHostMsg_FormsSeen,
                    std::vector<autofill::FormData> ,
                    base::TimeTicks ,
                    autofill::FormsSeenState )

IPC_MESSAGE_ROUTED1(AutofillHostMsg_PasswordFormsParsed,
                    std::vector<autofill::PasswordForm> )

IPC_MESSAGE_ROUTED1(AutofillHostMsg_PasswordFormsRendered,
                    std::vector<autofill::PasswordForm> )

IPC_MESSAGE_ROUTED1(AutofillHostMsg_PasswordFormSubmitted,
                    autofill::PasswordForm )

IPC_MESSAGE_ROUTED2(AutofillHostMsg_FormSubmitted,
                    autofill::FormData ,
                    base::TimeTicks )

IPC_MESSAGE_ROUTED3(AutofillHostMsg_TextFieldDidChange,
                    autofill::FormData ,
                    autofill::FormFieldData ,
                    base::TimeTicks )

IPC_MESSAGE_ROUTED5(AutofillHostMsg_QueryFormFieldAutofill,
                    int ,
                    autofill::FormData ,
                    autofill::FormFieldData ,
                    gfx::RectF ,
                    bool )

IPC_MESSAGE_ROUTED4(AutofillHostMsg_FillAutofillFormData,
                    int ,
                    autofill::FormData ,
                    autofill::FormFieldData ,
                    int )

IPC_MESSAGE_ROUTED0(AutofillHostMsg_DidPreviewAutofillFormData)

IPC_MESSAGE_ROUTED1(AutofillHostMsg_DidFillAutofillFormData,
                    base::TimeTicks )

IPC_MESSAGE_ROUTED2(AutofillHostMsg_RequestAutocomplete,
                    autofill::FormData ,
                    GURL )

IPC_MESSAGE_ROUTED0(AutofillHostMsg_ShowAutofillDialog)

IPC_MESSAGE_ROUTED0(AutofillHostMsg_DidEndTextFieldEditing)

IPC_MESSAGE_ROUTED0(AutofillHostMsg_HideAutofillUI)

IPC_MESSAGE_ROUTED3(AutofillHostMsg_ShowPasswordGenerationPopup,
                    gfx::Rect ,
                    int ,
                    autofill::PasswordForm)

IPC_MESSAGE_ROUTED2(AutofillHostMsg_AddPasswordFormMapping,
                    autofill::FormFieldData, 
                    autofill::PasswordFormFillData )

IPC_MESSAGE_ROUTED4(AutofillHostMsg_ShowPasswordSuggestions,
                    autofill::FormFieldData ,
                    gfx::RectF ,
                    std::vector<base::string16> ,
                    std::vector<base::string16> )

IPC_MESSAGE_ROUTED2(AutofillHostMsg_SetDataList,
                    std::vector<base::string16> ,
                    std::vector<base::string16> )
