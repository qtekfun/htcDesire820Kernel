// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_HOST_WINDOW_H_
#define REMOTING_HOST_HOST_WINDOW_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"

namespace remoting {

class ClientSessionControl;

class HostWindow : public base::NonThreadSafe {
 public:
  virtual ~HostWindow() {}

  
  static scoped_ptr<HostWindow> CreateContinueWindow();

  
  static scoped_ptr<HostWindow> CreateDisconnectWindow();

  
  
  virtual void Start(
      const base::WeakPtr<ClientSessionControl>& client_session_control) = 0;

 protected:
  HostWindow() {}

 private:
  
  
  friend class HostWindowProxy;

  DISALLOW_COPY_AND_ASSIGN(HostWindow);
};

}  

#endif  
