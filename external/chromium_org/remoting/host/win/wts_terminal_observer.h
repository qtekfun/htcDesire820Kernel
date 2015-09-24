// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_WIN_WTS_TERMINAL_OBSERVER_H_
#define REMOTING_HOST_WIN_WTS_TERMINAL_OBSERVER_H_

#include <windows.h>

#include "base/basictypes.h"

namespace remoting {

class WtsTerminalObserver {
 public:
  virtual ~WtsTerminalObserver() {}

  
  virtual void OnSessionAttached(uint32 session_id) = 0;

  
  virtual void OnSessionDetached() = 0;

 protected:
  WtsTerminalObserver() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(WtsTerminalObserver);
};

}  

#endif  
