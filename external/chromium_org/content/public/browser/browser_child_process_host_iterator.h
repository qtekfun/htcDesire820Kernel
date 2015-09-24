// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_BROWSER_CHILD_PROCESS_HOST_ITERATOR_H_
#define CONTENT_PUBLIC_BROWSER_BROWSER_CHILD_PROCESS_HOST_ITERATOR_H_

#include <list>

#include "content/common/content_export.h"

namespace IPC {
class Message;
}

namespace content {
class BrowserChildProcessHostDelegate;
class BrowserChildProcessHostImpl;
struct ChildProcessData;

class CONTENT_EXPORT BrowserChildProcessHostIterator {
 public:
  BrowserChildProcessHostIterator();
  explicit BrowserChildProcessHostIterator(int type);
  ~BrowserChildProcessHostIterator();

  
  
  bool operator++();
  bool Done();
  const ChildProcessData& GetData();
  bool Send(IPC::Message* message);
  BrowserChildProcessHostDelegate* GetDelegate();

 private:
  bool all_;
  int process_type_;
  std::list<BrowserChildProcessHostImpl*>::iterator iterator_;
};

template <class T>
class CONTENT_EXPORT BrowserChildProcessHostTypeIterator
    : public BrowserChildProcessHostIterator {
 public:
  explicit BrowserChildProcessHostTypeIterator(int process_type)
      : BrowserChildProcessHostIterator(process_type) {}
  T* operator->() {
    return static_cast<T*>(GetDelegate());
  }
  T* operator*() {
    return static_cast<T*>(GetDelegate());
  }
};

};  

#endif  
