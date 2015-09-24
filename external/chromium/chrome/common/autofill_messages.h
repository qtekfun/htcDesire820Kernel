// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <string>

#include "content/common/webkit_param_traits.h"
#include "ipc/ipc_message_macros.h"
#include "webkit/glue/form_data.h"
#include "webkit/glue/form_field.h"
#include "webkit/glue/password_form.h"
#include "webkit/glue/password_form_dom_manager.h"

#define IPC_MESSAGE_START AutofillMsgStart

IPC_STRUCT_TRAITS_BEGIN(webkit_glue::FormField)
  IPC_STRUCT_TRAITS_MEMBER(label)
  IPC_STRUCT_TRAITS_MEMBER(name)
  IPC_STRUCT_TRAITS_MEMBER(value)
  IPC_STRUCT_TRAITS_MEMBER(form_control_type)
  IPC_STRUCT_TRAITS_MEMBER(max_length)
  IPC_STRUCT_TRAITS_MEMBER(is_autofilled)
  IPC_STRUCT_TRAITS_MEMBER(option_strings)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(webkit_glue::FormData)
  IPC_STRUCT_TRAITS_MEMBER(name)
  IPC_STRUCT_TRAITS_MEMBER(method)
  IPC_STRUCT_TRAITS_MEMBER(origin)
  IPC_STRUCT_TRAITS_MEMBER(action)
  IPC_STRUCT_TRAITS_MEMBER(user_submitted)
  IPC_STRUCT_TRAITS_MEMBER(fields)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(webkit_glue::PasswordFormFillData)
  IPC_STRUCT_TRAITS_MEMBER(basic_data)
  IPC_STRUCT_TRAITS_MEMBER(additional_logins)
  IPC_STRUCT_TRAITS_MEMBER(wait_for_username)
IPC_STRUCT_TRAITS_END()


IPC_MESSAGE_ROUTED5(AutofillMsg_SuggestionsReturned,
                    int ,
                    std::vector<string16> ,
                    std::vector<string16> ,
                    std::vector<string16> ,
                    std::vector<int> )

IPC_MESSAGE_ROUTED2(AutofillMsg_FormDataFilled,
                    int ,
                    webkit_glue::FormData )

IPC_MESSAGE_ROUTED1(AutofillMsg_FillPasswordForm,
                    webkit_glue::PasswordFormFillData)



IPC_MESSAGE_ROUTED1(AutofillHostMsg_FormsSeen,
                    std::vector<webkit_glue::FormData> )

IPC_MESSAGE_ROUTED1(AutofillHostMsg_PasswordFormsFound,
                    std::vector<webkit_glue::PasswordForm> )

IPC_MESSAGE_ROUTED1(AutofillHostMsg_PasswordFormsVisible,
                    std::vector<webkit_glue::PasswordForm> )

IPC_MESSAGE_ROUTED1(AutofillHostMsg_FormSubmitted,
                    webkit_glue::FormData )

IPC_MESSAGE_ROUTED3(AutofillHostMsg_QueryFormFieldAutofill,
                    int ,
                    webkit_glue::FormData ,
                    webkit_glue::FormField )

IPC_MESSAGE_ROUTED0(AutofillHostMsg_DidShowAutofillSuggestions)

IPC_MESSAGE_ROUTED4(AutofillHostMsg_FillAutofillFormData,
                    int ,
                    webkit_glue::FormData ,
                    webkit_glue::FormField ,
                    int )

IPC_MESSAGE_ROUTED0(AutofillHostMsg_DidFillAutofillFormData)

IPC_MESSAGE_ROUTED2(AutofillHostMsg_RemoveAutocompleteEntry,
                    string16 ,
                    string16 )

IPC_MESSAGE_ROUTED0(AutofillHostMsg_ShowAutofillDialog)


