// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_FRAME_TEST_IE_CONFIGURATOR_H_
#define CHROME_FRAME_TEST_IE_CONFIGURATOR_H_

#include "base/basictypes.h"

namespace chrome_frame_test {

class IEConfigurator {
 public:
  virtual ~IEConfigurator();

  
  
  virtual void Initialize() = 0;

  
  virtual void ApplySettings() = 0;

  
  virtual void RevertSettings() = 0;

 protected:
  IEConfigurator();
};

IEConfigurator* CreateConfigurator();

void InstallIEConfigurator();

}  

#endif  
