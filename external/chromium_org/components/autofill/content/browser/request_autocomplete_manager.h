// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_BROWSER_REQUEST_AUTOCOMPLETE_MANAGER_H_
#define COMPONENTS_AUTOFILL_CONTENT_BROWSER_REQUEST_AUTOCOMPLETE_MANAGER_H_

#include "base/callback_forward.h"
#include "base/memory/weak_ptr.h"
#include "third_party/WebKit/public/web/WebFormElement.h"

class GURL;

namespace autofill {

class AutofillDriverImpl;
struct FormData;
class FormStructure;

class RequestAutocompleteManager {
 public:
  explicit RequestAutocompleteManager(AutofillDriverImpl* autofill_driver);
  ~RequestAutocompleteManager();

  
  
  void OnRequestAutocomplete(const FormData& form, const GURL& frame_url);

 private:
  
  
  
  void ReturnAutocompleteResult(
      blink::WebFormElement::AutocompleteResult result,
      const FormData& form_data);

  
  
  void ShowRequestAutocompleteDialog(
      const FormData& form,
      const GURL& source_url,
      const base::Callback<void(const FormStructure*)>& callback);

  
  
  
  
  void ReturnAutocompleteData(const FormStructure* result);

  
  AutofillDriverImpl* const autofill_driver_;  

  base::WeakPtrFactory<RequestAutocompleteManager> weak_ptr_factory_;
};

}  

#endif  
