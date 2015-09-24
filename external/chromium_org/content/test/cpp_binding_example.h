// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_CPP_BINDING_EXAMPLE_H_
#define CONTENT_TEST_CPP_BINDING_EXAMPLE_H_


#include "webkit/renderer/cpp_bound_class.h"

namespace content {

class CppBindingExample : public webkit_glue::CppBoundClass {
 public:
  
  
  CppBindingExample();

  
  
  
  
  
  
  
  
  
  

  
  void echoValue(const webkit_glue::CppArgumentList& args,
                 webkit_glue::CppVariant* result);

  
  
  void echoType(const webkit_glue::CppArgumentList& args,
                webkit_glue::CppVariant* result);

  
  
  void plus(const webkit_glue::CppArgumentList& args,
            webkit_glue::CppVariant* result);

  
  void same(webkit_glue::CppVariant* result);

  
  
  void fallbackMethod(const webkit_glue::CppArgumentList& args,
                      webkit_glue::CppVariant* result);

  
  webkit_glue::CppVariant my_value;
  webkit_glue::CppVariant my_other_value;
};

}  

#endif  
