// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_TYPE_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_TYPE_H_

#include <string>

#include "components/autofill/core/browser/field_types.h"

namespace autofill {

class AutofillType {
 public:
  explicit AutofillType(ServerFieldType field_type);
  AutofillType(HtmlFieldType field_type, HtmlFieldMode mode);
  AutofillType(const AutofillType& autofill_type);
  AutofillType& operator=(const AutofillType& autofill_type);

  ServerFieldType server_type() const { return server_type_; }
  HtmlFieldType html_type() const { return html_type_; }

  FieldTypeGroup group() const;

  
  
  bool IsUnknown() const;

  
  
  
  
  
  
  ServerFieldType GetStorableType() const;

  
  std::string ToString() const;

  
  
  static ServerFieldType GetEquivalentBillingFieldType(
      ServerFieldType field_type);

 private:
  
  ServerFieldType server_type_;

  
  
  HtmlFieldType html_type_;
  HtmlFieldMode html_mode_;
};

}  

#endif  
