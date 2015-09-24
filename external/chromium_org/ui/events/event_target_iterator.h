// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_EVENT_TARGET_ITERATOR_H_
#define UI_EVENTS_EVENT_TARGET_ITERATOR_H_

#include <vector>

namespace ui {

class EventTarget;

class EventTargetIterator {
 public:
  virtual ~EventTargetIterator() {}
  virtual EventTarget* GetNextTarget() = 0;
};

template<typename T>
class EventTargetIteratorImpl : public EventTargetIterator {
 public:
  explicit EventTargetIteratorImpl(const std::vector<T*>& children)
      : begin_(children.rbegin()),
        end_(children.rend()) {
  }
  virtual ~EventTargetIteratorImpl() {}

  virtual EventTarget* GetNextTarget() OVERRIDE {
    if (begin_ == end_)
      return NULL;
    EventTarget* target = *(begin_);
    ++begin_;
    return target;
  }

 private:
  typename std::vector<T*>::const_reverse_iterator begin_;
  typename std::vector<T*>::const_reverse_iterator end_;
};

}  

#endif  
