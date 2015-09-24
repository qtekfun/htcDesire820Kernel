// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_DEBUG_LEAK_TRACKER_H_
#define BASE_DEBUG_LEAK_TRACKER_H_

#ifndef NDEBUG
#define ENABLE_LEAK_TRACKER
#endif

#ifdef ENABLE_LEAK_TRACKER
#include "base/containers/linked_list.h"
#include "base/debug/stack_trace.h"
#include "base/logging.h"
#endif  


namespace base {
namespace debug {

#ifndef ENABLE_LEAK_TRACKER

template<typename T>
class LeakTracker {
 public:
  ~LeakTracker() {}
  static void CheckForLeaks() {}
  static int NumLiveInstances() { return -1; }
};

#else


template<typename T>
class LeakTracker : public LinkNode<LeakTracker<T> > {
 public:
  LeakTracker() {
    instances()->Append(this);
  }

  ~LeakTracker() {
    this->RemoveFromList();
  }

  static void CheckForLeaks() {
    
    size_t count = 0;

    
    
    
    const size_t kMaxStackTracesToCopyOntoStack = 3;
    StackTrace stacktraces[kMaxStackTracesToCopyOntoStack];

    for (LinkNode<LeakTracker<T> >* node = instances()->head();
         node != instances()->end();
         node = node->next()) {
      StackTrace& allocation_stack = node->value()->allocation_stack_;

      if (count < kMaxStackTracesToCopyOntoStack)
        stacktraces[count] = allocation_stack;

      ++count;
      if (LOG_IS_ON(ERROR)) {
        LOG_STREAM(ERROR) << "Leaked " << node << " which was allocated by:";
        allocation_stack.OutputToStream(&LOG_STREAM(ERROR));
      }
    }

    CHECK_EQ(0u, count);

    
    
    if (count == 0x1234) {
      for (size_t i = 0; i < kMaxStackTracesToCopyOntoStack; ++i)
        stacktraces[i].Print();
    }
  }

  static int NumLiveInstances() {
    
    int count = 0;
    for (LinkNode<LeakTracker<T> >* node = instances()->head();
         node != instances()->end();
         node = node->next()) {
      ++count;
    }
    return count;
  }

 private:
  
  static LinkedList<LeakTracker<T> >* instances() {
    static LinkedList<LeakTracker<T> > list;
    return &list;
  }

  StackTrace allocation_stack_;
};

#endif  

}  
}  

#endif  
