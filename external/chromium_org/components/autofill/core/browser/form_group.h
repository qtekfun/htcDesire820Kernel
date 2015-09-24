// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_FORM_GROUP_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_FORM_GROUP_H_

#include <string>

#include "base/strings/string16.h"
#include "components/autofill/core/browser/field_types.h"

namespace autofill {

class AutofillType;

class FormGroup {
 public:
  virtual ~FormGroup() {}

  
  
  
  
  virtual void GetMatchingTypes(const base::string16& text,
                                const std::string& app_locale,
                                ServerFieldTypeSet* matching_types) const;

  
  
  virtual void GetNonEmptyTypes(const std::string& app_locale,
                                ServerFieldTypeSet* non_empty_types) const;

  
  
  virtual base::string16 GetRawInfo(ServerFieldType type) const = 0;

  
  
  
  virtual void SetRawInfo(ServerFieldType type,
                          const base::string16& value) = 0;

  
  
  virtual base::string16 GetInfo(const AutofillType& type,
                                 const std::string& app_locale) const;

  
  
  virtual bool SetInfo(const AutofillType& type,
                       const base::string16& value,
                       const std::string& app_locale);

 protected:
  
  
  friend class AutofillProfile;

  
  
  virtual void GetSupportedTypes(ServerFieldTypeSet* supported_types) const = 0;
};

}  

#endif  
