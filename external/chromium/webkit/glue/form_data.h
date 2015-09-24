// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_FORM_DATA_H__
#define WEBKIT_GLUE_FORM_DATA_H__

#include <vector>

#ifdef ANDROID
#include "base/base_api.h"
#endif
#include "base/string_util.h"
#include "googleurl/src/gurl.h"
#include "webkit/glue/form_field.h"

namespace webkit_glue {

struct 
#ifdef ANDROID
BASE_API
#endif
FormData {
  
  string16 name;
  
  string16 method;
  
  GURL origin;
  
  GURL action;
  
  bool user_submitted;
  
  std::vector<FormField> fields;

  FormData();
  FormData(const FormData& data);
  ~FormData();

  
  bool operator==(const FormData& form) const;
};

}  

#endif  
