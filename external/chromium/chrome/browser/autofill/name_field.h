// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_NAME_FIELD_H_
#define CHROME_BROWSER_AUTOFILL_NAME_FIELD_H_
#pragma once

#include <vector>

#include "chrome/browser/autofill/autofill_field.h"
#include "chrome/browser/autofill/form_field.h"

class NameField : public FormField {
 public:
  static NameField* Parse(std::vector<AutofillField*>::const_iterator* iter,
                          bool is_ecml);

 protected:
  NameField() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(NameField);
};

class FullNameField : public NameField {
 public:
  virtual bool GetFieldInfo(FieldTypeMap* field_type_map) const;

  static FullNameField* Parse(
      std::vector<AutofillField*>::const_iterator* iter);

 private:
  explicit FullNameField(AutofillField* field);

  AutofillField* field_;
  DISALLOW_COPY_AND_ASSIGN(FullNameField);
};

class FirstLastNameField : public NameField {
 public:
  static FirstLastNameField* Parse1(
      std::vector<AutofillField*>::const_iterator* iter);
  static FirstLastNameField* Parse2(
      std::vector<AutofillField*>::const_iterator* iter);
  static FirstLastNameField* ParseEcmlName(
      std::vector<AutofillField*>::const_iterator* iter);
  static FirstLastNameField* Parse(
      std::vector<AutofillField*>::const_iterator* iter, bool is_ecml);

  virtual bool GetFieldInfo(FieldTypeMap* field_type_map) const;

 private:
  FirstLastNameField();

  AutofillField* first_name_;
  AutofillField* middle_name_;  
  AutofillField* last_name_;
  bool middle_initial_;  

  DISALLOW_COPY_AND_ASSIGN(FirstLastNameField);
};

#endif  
