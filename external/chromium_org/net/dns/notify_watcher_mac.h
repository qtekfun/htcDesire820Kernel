// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_NOTIFY_WATCHER_MAC_H_
#define NET_DNS_NOTIFY_WATCHER_MAC_H_

#include "base/callback.h"
#include "base/message_loop/message_loop.h"

namespace net {

class NotifyWatcherMac : public base::MessageLoopForIO::Watcher {
 public:
  
  typedef base::Callback<void(bool succeeded)> CallbackType;

  NotifyWatcherMac();

  
  virtual ~NotifyWatcherMac();

  
  
  bool Watch(const char* key, const CallbackType& callback);

  
  void Cancel();

 private:
  
  virtual void OnFileCanReadWithoutBlocking(int fd) OVERRIDE;
  virtual void OnFileCanWriteWithoutBlocking(int fd) OVERRIDE {}

  int notify_fd_;
  int notify_token_;
  CallbackType callback_;
  base::MessageLoopForIO::FileDescriptorWatcher watcher_;

  DISALLOW_COPY_AND_ASSIGN(NotifyWatcherMac);
};

}  

#endif  
