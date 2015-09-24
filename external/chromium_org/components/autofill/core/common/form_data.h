// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_COMMON_FORM_DATA_H__
#define COMPONENTS_AUTOFILL_CORE_COMMON_FORM_DATA_H__

#include <vector>

#include "base/strings/string16.h"
#include "components/autofill/core/common/form_field_data.h"
#include "url/gurl.h"

namespace autofill {

struct FormData {
  FormData();
  FormData(const FormData& data);
  ~FormData();

  
  bool operator==(const FormData& form) const;
  bool operator!=(const FormData& form) const;

  
  base::string16 name;
  
  base::string16 method;
  
  GURL origin;
  
  GURL action;
  
  bool user_submitted;
  
  std::vector<FormFieldData> fields;
};

std::ostream& operator<<(std::ostream& os, const FormData& form);

void SerializeFormData(const FormData& form_data, Pickle* pickle);
bool DeserializeFormData(PickleIterator* iter, FormData* form_data);

}  

#endif  
