// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_FIELD_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_FIELD_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "components/autofill/core/browser/field_types.h"
#include "components/autofill/core/common/form_field_data.h"

namespace autofill {

class AutofillType;

class AutofillField : public FormFieldData {
 public:
  enum PhonePart {
    IGNORED = 0,
    PHONE_PREFIX = 1,
    PHONE_SUFFIX = 2,
  };

  AutofillField();
  AutofillField(const FormFieldData& field, const base::string16& unique_name);
  virtual ~AutofillField();

  const base::string16& unique_name() const { return unique_name_; }

  const std::string& section() const { return section_; }
  ServerFieldType heuristic_type() const { return heuristic_type_; }
  ServerFieldType server_type() const { return server_type_; }
  HtmlFieldType html_type() const { return html_type_; }
  HtmlFieldMode html_mode() const { return html_mode_; }
  const ServerFieldTypeSet& possible_types() const { return possible_types_; }
  PhonePart phone_part() const { return phone_part_; }

  
  void set_section(const std::string& section) { section_ = section; }
  void set_heuristic_type(ServerFieldType type);
  void set_server_type(ServerFieldType type);
  void set_possible_types(const ServerFieldTypeSet& possible_types) {
    possible_types_ = possible_types;
  }
  void SetHtmlType(HtmlFieldType type, HtmlFieldMode mode);

  
  
  AutofillType Type() const;

  
  bool IsEmpty() const;

  
  
  std::string FieldSignature() const;

  
  
  bool IsFieldFillable() const;

  void set_default_value(const std::string& value) { default_value_ = value; }
  const std::string& default_value() const { return default_value_; }

  
  
  
  static void FillFormField(const AutofillField& field,
                            const base::string16& value,
                            const std::string& app_locale,
                            FormFieldData* field_data);

 private:
  
  base::string16 unique_name_;

  
  
  std::string section_;

  
  ServerFieldType server_type_;

  
  ServerFieldType heuristic_type_;

  
  HtmlFieldType html_type_;

  
  
  HtmlFieldMode html_mode_;

  
  ServerFieldTypeSet possible_types_;

  
  PhonePart phone_part_;

  
  std::string default_value_;

  DISALLOW_COPY_AND_ASSIGN(AutofillField);
};

}  

#endif  
