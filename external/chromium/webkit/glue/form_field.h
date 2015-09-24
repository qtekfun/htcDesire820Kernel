// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_FORM_FIELD_H_
#define WEBKIT_GLUE_FORM_FIELD_H_

#include <vector>

#ifdef ANDROID
#include "base/base_api.h"
#endif
#include "base/string16.h"
#ifndef ANDROID
#include "third_party/WebKit/Source/WebKit/chromium/public/WebFormControlElement.h"
#endif

namespace webkit_glue {

struct
#ifdef ANDROID
BASE_API
#endif
FormField {
  FormField();
#ifndef ANDROID
  explicit FormField(WebKit::WebFormControlElement element);
#endif
  FormField(const string16& label,
            const string16& name,
            const string16& value,
            const string16& form_control_type,
            int max_length,
            bool is_autofilled);
  virtual ~FormField();

  
  
  
  
  bool operator==(const FormField& field) const;
  bool operator!=(const FormField& field) const;

  
  
  bool StrictlyEqualsHack(const FormField& field) const;

  string16 label;
  string16 name;
  string16 value;
  string16 form_control_type;
  int max_length;
  bool is_autofilled;
  std::vector<string16> option_strings;
};

std::ostream& operator<<(std::ostream& os, const FormField& field);

}  

#endif  
