// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CPP_BINDING_EXAMPLE_H__
#define CPP_BINDING_EXAMPLE_H__

#include "webkit/glue/cpp_bound_class.h"

class CppBindingExample : public CppBoundClass {
 public:
  
  
  CppBindingExample();

  
  
  
  
  
  
  
  
  
  

  
  void echoValue(const CppArgumentList& args, CppVariant* result);

  
  
  void echoType(const CppArgumentList& args, CppVariant* result);

  
  
  void plus(const CppArgumentList& args, CppVariant* result);

  
  void same(CppVariant* result);

  
  
  void fallbackMethod(const CppArgumentList& args, CppVariant* result);

  
  CppVariant my_value;
  CppVariant my_other_value;
};

#endif  
