// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_INFOBARS_INTERNAL_HOST_WINDOW_MANAGER_H_
#define CHROME_FRAME_INFOBARS_INTERNAL_HOST_WINDOW_MANAGER_H_

#include "base/basictypes.h"
#include "chrome_frame/infobars/internal/subclassing_window_with_delegate.h"

class DisplacedWindowManager;

class HostWindowManager
    : public SubclassingWindowWithDelegate<HostWindowManager> {
 public:
  HostWindowManager();
  virtual ~HostWindowManager();

  
  
  
  void UpdateLayout();

 private:
  class DisplacedWindowDelegate;
  friend class DisplacedWindowDelegate;

  
  
  
  bool FindDisplacedWindow(HWND old_window);

  
  DisplacedWindowManager* displaced_window_manager_;

  DISALLOW_COPY_AND_ASSIGN(HostWindowManager);
};  

#endif  
