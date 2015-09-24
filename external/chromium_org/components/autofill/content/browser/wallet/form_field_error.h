// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_BROWSER_WALLET_FORM_FIELD_ERROR_H_
#define COMPONENTS_AUTOFILL_BROWSER_WALLET_FORM_FIELD_ERROR_H_

#include "base/strings/string16.h"
#include "components/autofill/core/browser/field_types.h"

namespace base {
class DictionaryValue;
}

namespace autofill {
namespace wallet {

class FormFieldError {
 public:
  
  enum ErrorType {
    UNKNOWN_ERROR,
    INVALID_PHONE_NUMBER,
    INVALID_POSTAL_CODE,
    
    INVALID_ADDRESS,
    
    INVALID_CARD_DETAILS,
    
    INVALID_CITY,
    
    
    
    INVALID_INSTRUMENT,
    
    INVALID_STATE,
    REQUIRED_FIELD_NOT_SET,
    
    
  };

  
  enum Location {
    UNKNOWN_LOCATION,
    PAYMENT_INSTRUMENT,
    SHIPPING_ADDRESS,
    
    
    
    
    
    LEGAL_ADDRESS,
  };

  FormFieldError(ErrorType error_type, Location location);
  ~FormFieldError();

  ErrorType error_type() const { return error_type_; }
  Location location() const { return location_; }

  
  ServerFieldType GetAutofillType() const;

  
  
  base::string16 GetErrorMessage() const;

  
  static FormFieldError CreateFormFieldError(
      const base::DictionaryValue& dictionary);

  bool operator==(const FormFieldError& other) const;

 private:
  
  ErrorType error_type_;

  
  Location location_;

  
};

}  
}  

#endif  
