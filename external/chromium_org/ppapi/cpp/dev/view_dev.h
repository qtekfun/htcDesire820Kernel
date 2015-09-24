// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_VIEW_DEV_H_
#define PPAPI_CPP_DEV_VIEW_DEV_H_

#include "ppapi/cpp/view.h"

namespace pp {

class ViewDev : public View {
 public:
  ViewDev() : View() {}
  ViewDev(const View& other) : View(other) {}

  virtual ~ViewDev() {}

  
  
  
  
  
  
  
  
  
  
  float GetDeviceScale() const;

  
  
  
  
  
  
  float GetCSSScale() const;
};

}  

#endif  
