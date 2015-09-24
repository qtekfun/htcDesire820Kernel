// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_TEST_DISPLAY_MANAGER_TEST_API_H_
#define ASH_TEST_DISPLAY_MANAGER_TEST_API_H_

#include <string>

#include "base/basictypes.h"

namespace ash {
namespace internal {
class DisplayManager;
}  

namespace test {

class DisplayManagerTestApi {
 public:
  explicit DisplayManagerTestApi(internal::DisplayManager* display_manager);
  virtual ~DisplayManagerTestApi();

  
  
  
  
  void UpdateDisplay(const std::string& display_specs);

  
  
  int64 SetFirstDisplayAsInternalDisplay();

  
  void DisableChangeDisplayUponHostResize();

 private:
  internal::DisplayManager* display_manager_;  

  DISALLOW_COPY_AND_ASSIGN(DisplayManagerTestApi);
};

}  
}  

#endif  
