// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_AUTOFILL_FIELD_H_
#define CHROME_BROWSER_AUTOFILL_AUTOFILL_FIELD_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/string16.h"
#include "chrome/browser/autofill/field_types.h"
#include "webkit/glue/form_field.h"

class AutofillField : public webkit_glue::FormField {
 public:
  AutofillField();
  AutofillField(const webkit_glue::FormField& field,
                const string16& unique_name);
  virtual ~AutofillField();

  const string16& unique_name() const { return unique_name_; }

  AutofillFieldType heuristic_type() const { return heuristic_type_; }
  AutofillFieldType server_type() const { return server_type_; }
  const FieldTypeSet& possible_types() const { return possible_types_; }

  
  void set_heuristic_type(AutofillFieldType type);
  void set_server_type(AutofillFieldType type) { server_type_ = type; }
  void set_possible_types(const FieldTypeSet& possible_types) {
    possible_types_ = possible_types;
  }

  
  
  AutofillFieldType type() const;

  
  bool IsEmpty() const;

  
  
  std::string FieldSignature() const;

  
  
  bool IsFieldFillable() const;

 private:
  
  string16 unique_name_;

  
  AutofillFieldType server_type_;

  
  AutofillFieldType heuristic_type_;

  
  FieldTypeSet possible_types_;

  DISALLOW_COPY_AND_ASSIGN(AutofillField);
};

#endif  
