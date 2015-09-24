// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_DESKTOP_SESSION_H_
#define REMOTING_HOST_DESKTOP_SESSION_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"

namespace remoting {

class DaemonProcess;
class ScreenResolution;

class DesktopSession {
 public:
  virtual ~DesktopSession();

  
  virtual void SetScreenResolution(const ScreenResolution& resolution) = 0;

  int id() const { return id_; }

 protected:
  
  
  DesktopSession(DaemonProcess* daemon_process, int id);

  DaemonProcess* daemon_process() const { return daemon_process_; }

 private:
  
  DaemonProcess* const daemon_process_;

  
  const int id_;

  DISALLOW_COPY_AND_ASSIGN(DesktopSession);
};

}  

#endif  
