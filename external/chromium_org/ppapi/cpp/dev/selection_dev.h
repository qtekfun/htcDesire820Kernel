// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_SELECTION_DEV_H_
#define PPAPI_CPP_DEV_SELECTION_DEV_H_

#include "ppapi/c/dev/ppp_selection_dev.h"
#include "ppapi/cpp/instance_handle.h"

namespace pp {

class Var;

class Selection_Dev {
 public:
  explicit Selection_Dev(Instance* instance);
  virtual ~Selection_Dev();

  
  
  virtual Var GetSelectedText(bool html) = 0;

 private:
  InstanceHandle associated_instance_;
};

}  

#endif  
