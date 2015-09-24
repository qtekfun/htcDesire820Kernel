// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_DAEMON_INSTALLER_WIN_H_
#define REMOTING_HOST_DAEMON_INSTALLER_WIN_H_

#include <objbase.h>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"

namespace remoting {

class DaemonInstallerWin {
 public:
  typedef base::Callback<void (HRESULT result)> CompletionCallback;

  virtual ~DaemonInstallerWin();

  
  virtual void Install() = 0;

  
  
  
  static scoped_ptr<DaemonInstallerWin> Create(HWND window_handle,
                                               CompletionCallback done);

 protected:
  DaemonInstallerWin(const CompletionCallback& done);

  
  void Done(HRESULT result);

 private:
  
  
  CompletionCallback done_;

  DISALLOW_COPY_AND_ASSIGN(DaemonInstallerWin);
};

HWND GetTopLevelWindow(HWND window);

}  

#endif  
