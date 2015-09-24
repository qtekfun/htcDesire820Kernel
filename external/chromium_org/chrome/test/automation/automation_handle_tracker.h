// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_TEST_AUTOMATION_AUTOMATION_HANDLE_TRACKER_H__
#define CHROME_TEST_AUTOMATION_AUTOMATION_HANDLE_TRACKER_H__

#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "ipc/ipc_channel.h"

typedef int AutomationHandle;

class AutomationHandleTracker;
class AutomationMessageSender;

class AutomationResourceProxy
    : public base::RefCountedThreadSafe<AutomationResourceProxy> {
 public:
  AutomationResourceProxy(AutomationHandleTracker* tracker,
                          AutomationMessageSender* sender,
                          AutomationHandle handle);

  
  
  void Invalidate() {
    is_valid_ = false;
    tracker_ = NULL;
  }

  bool is_valid() const { return is_valid_; }

  
  AutomationHandle handle() { return handle_; }

 protected:
  friend class AutomationHandleTracker;
  friend class base::RefCountedThreadSafe<AutomationResourceProxy>;

  virtual ~AutomationResourceProxy();

  AutomationHandle handle_;

  
  
  AutomationHandleTracker* tracker_;

  
  AutomationMessageSender* sender_;

 private:
  
  
  bool is_valid_;

  DISALLOW_COPY_AND_ASSIGN(AutomationResourceProxy);
};

class AutomationHandleTracker {
 public:
  explicit AutomationHandleTracker();
  ~AutomationHandleTracker();

  
  void Add(AutomationResourceProxy* proxy);

  
  
  
  
  void Remove(AutomationResourceProxy* proxy);

  
  
  
  
  void InvalidateHandle(AutomationHandle handle);

  AutomationResourceProxy* GetResource(AutomationHandle handle);

  void put_channel(IPC::Channel* channel) {
    channel_ = channel;
  }

 private:
  typedef
    std::map<AutomationHandle, scoped_refptr<AutomationResourceProxy> >
        HandleToObjectMap;
  typedef std::pair<AutomationHandle, AutomationResourceProxy*> MapEntry;

  HandleToObjectMap handle_to_object_;

  base::Lock map_lock_;
  IPC::Channel* channel_;
  DISALLOW_COPY_AND_ASSIGN(AutomationHandleTracker);
};

#endif  
