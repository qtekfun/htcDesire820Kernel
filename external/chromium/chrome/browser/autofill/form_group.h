// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_FORM_GROUP_H_
#define CHROME_BROWSER_AUTOFILL_FORM_GROUP_H_
#pragma once

#include <vector>

#include "base/string16.h"
#include "base/string_util.h"
#include "chrome/browser/autofill/field_types.h"

class FormGroup {
 public:
  virtual ~FormGroup() {}

  
  
  
  virtual void GetPossibleFieldTypes(const string16& text,
                                     FieldTypeSet* possible_types) const = 0;

  
  
  virtual void GetAvailableFieldTypes(FieldTypeSet* available_types) const = 0;

  
  
  virtual string16 GetInfo(AutofillFieldType type) const = 0;

  
  virtual void SetInfo(AutofillFieldType type, const string16& value) = 0;

  
  
  virtual const string16 Label() const;

  
  
  virtual bool operator!=(const FormGroup& form_group) const;

  
  
  bool IsSubsetOf(const FormGroup& form_group) const;

  
  
  bool IntersectionOfTypesHasEqualValues(const FormGroup& form_group) const;

  
  void MergeWith(const FormGroup& form_group);

  
  void OverwriteWith(const FormGroup& form_group);
};

#endif  
