// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_WIN_WTS_TERMINAL_MONITOR_H_
#define REMOTING_HOST_WIN_WTS_TERMINAL_MONITOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/utf_string_conversions.h"

namespace remoting {

class WtsTerminalObserver;

extern const uint32 kInvalidSessionId;

class WtsTerminalMonitor {
 public:
  
  static const char* kConsole;

  virtual ~WtsTerminalMonitor();

  
  
  
  
  
  
  
  virtual bool AddWtsTerminalObserver(const std::string& terminal_id,
                                      WtsTerminalObserver* observer) = 0;

  
  virtual void RemoveWtsTerminalObserver(WtsTerminalObserver* observer) = 0;

  
  
  
  static bool LookupTerminalId(uint32 session_id, std::string* terminal_id);

  
  
  
  static uint32 LookupSessionId(const std::string& terminal_id);

 protected:
  WtsTerminalMonitor();

 private:
  DISALLOW_COPY_AND_ASSIGN(WtsTerminalMonitor);
};

}  

#endif  
