// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_FIND_DEV_H_
#define PPAPI_CPP_DEV_FIND_DEV_H_

#include <string>

#include "ppapi/c/dev/ppp_find_dev.h"
#include "ppapi/cpp/instance_handle.h"

namespace pp {

class Instance;

class Find_Dev {
 public:
  
  Find_Dev(Instance* instance);
  virtual ~Find_Dev();

  
  
  virtual bool StartFind(const std::string& text, bool case_sensitive) = 0;
  virtual void SelectFindResult(bool forward) = 0;
  virtual void StopFind() = 0;

  
  void NumberOfFindResultsChanged(int32_t total, bool final_result);
  void SelectedFindResultChanged(int32_t index);

 private:
  InstanceHandle associated_instance_;
};

}  

#endif  
