// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_BROWSER_CHILD_PROCESS_OBSERVER_H_
#define CONTENT_PUBLIC_BROWSER_BROWSER_CHILD_PROCESS_OBSERVER_H_

#include "content/common/content_export.h"

namespace content {

struct ChildProcessData;

class CONTENT_EXPORT BrowserChildProcessObserver {
 public:
  
  virtual void BrowserChildProcessHostConnected(const ChildProcessData& data) {}

  
  virtual void BrowserChildProcessHostDisconnected(
      const ChildProcessData& data) {}

  
  virtual void BrowserChildProcessCrashed(const ChildProcessData& data) {}

  
  
  
  virtual void BrowserChildProcessInstanceCreated(
      const ChildProcessData& data) {}

 protected:
  
  virtual ~BrowserChildProcessObserver() {}

  static void Add(BrowserChildProcessObserver* observer);
  static void Remove(BrowserChildProcessObserver* observer);
};

}  

#endif  
