// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_WEBDATA_AUTOFILL_WEBDATA_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_WEBDATA_AUTOFILL_WEBDATA_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "components/webdata/common/web_data_service_base.h"

namespace base {

class Time;

}  

class Profile;
class WebDataServiceConsumer;

namespace autofill {

class AutofillProfile;
class CreditCard;
struct FormFieldData;

class AutofillWebData {
 public:
  virtual ~AutofillWebData() {}

  
  virtual void AddFormFields(
      const std::vector<FormFieldData>& fields) = 0;

  
  
  
  
  virtual WebDataServiceBase::Handle GetFormValuesForElementName(
      const base::string16& name,
      const base::string16& prefix,
      int limit,
      WebDataServiceConsumer* consumer) = 0;

  
  virtual WebDataServiceBase::Handle HasFormElements(
      WebDataServiceConsumer* consumer) = 0;

  
  virtual void RemoveFormElementsAddedBetween(
      const base::Time& delete_begin, const base::Time& delete_end) = 0;

  virtual void RemoveFormValueForElementName(const base::string16& name,
                                             const base::string16& value) = 0;

  
  virtual void AddAutofillProfile(const AutofillProfile& profile) = 0;

  
  virtual void UpdateAutofillProfile(const AutofillProfile& profile) = 0;

  
  
  virtual void RemoveAutofillProfile(const std::string& guid) = 0;

  
  
  
  
  virtual WebDataServiceBase::Handle GetAutofillProfiles(
      WebDataServiceConsumer* consumer) = 0;

  
  virtual void AddCreditCard(const CreditCard& credit_card) = 0;

  
  virtual void UpdateCreditCard(const CreditCard& credit_card) = 0;

  
  
  virtual void RemoveCreditCard(const std::string& guid) = 0;

  
  
  
  
  virtual WebDataServiceBase::Handle GetCreditCards(
      WebDataServiceConsumer* consumer) = 0;

  
  virtual void RemoveAutofillDataModifiedBetween(
      const base::Time& delete_begin, const base::Time& delete_end) = 0;

  
  
  virtual void RemoveOriginURLsModifiedBetween(
      const base::Time& delete_begin, const base::Time& delete_end) = 0;
};

}  

#endif  
