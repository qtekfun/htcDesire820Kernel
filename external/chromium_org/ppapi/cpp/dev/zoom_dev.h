// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_ZOOM_DEV_H_
#define PPAPI_CPP_DEV_ZOOM_DEV_H_

#include <string>

#include "ppapi/c/dev/ppp_zoom_dev.h"
#include "ppapi/cpp/instance_handle.h"

namespace pp {

class Instance;

class Zoom_Dev {
 public:
  explicit Zoom_Dev(Instance* instance);
  virtual ~Zoom_Dev();

  
  
  virtual void Zoom(double factor, bool text_only) = 0;

  
  void ZoomChanged(double factor);
  void ZoomLimitsChanged(double minimum_factor, double maximium_factor);

 private:
  InstanceHandle associated_instance_;
};

}  

#endif  
