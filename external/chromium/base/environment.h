// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_ENVIRONMENT_H_
#define BASE_ENVIRONMENT_H_
#pragma once

#include <string>

#include "base/base_api.h"
#include "build/build_config.h"

namespace base {

namespace env_vars {

#if defined(OS_POSIX)
extern const char kHome[];
#endif

}  

class BASE_API Environment {
 public:
  virtual ~Environment();

  
  
  static Environment* Create();

  
  
  virtual bool GetVar(const char* variable_name, std::string* result) = 0;

  
  virtual bool HasVar(const char* variable_name);

  
  virtual bool SetVar(const char* variable_name,
                      const std::string& new_value) = 0;

  
  virtual bool UnSetVar(const char* variable_name) = 0;
};

}  

#endif  
