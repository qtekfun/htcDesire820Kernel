// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_HOST_STATUS_MONITOR_H_
#define REMOTING_HOST_HOST_STATUS_MONITOR_H_

namespace remoting {

class HostStatusObserver;

class HostStatusMonitor {
 public:
  virtual ~HostStatusMonitor() {}

  
  virtual void AddStatusObserver(HostStatusObserver* observer) = 0;
  virtual void RemoveStatusObserver(HostStatusObserver* observer) = 0;
};

}  

#endif  
