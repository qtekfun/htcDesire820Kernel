// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_FULLSCREEN_H_
#define PPAPI_CPP_FULLSCREEN_H_

#include "ppapi/cpp/instance_handle.h"


namespace pp {

class Size;

class Fullscreen {
 public:
  
  
  
  
  explicit Fullscreen(const InstanceHandle& instance);

  
  virtual ~Fullscreen();

  
  
  
  
  
  bool IsFullscreen();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool SetFullscreen(bool fullscreen);

  
  
  
  
  
  
  
  
  bool GetScreenSize(Size* size);

 private:
  InstanceHandle instance_;
};

}  

#endif  
