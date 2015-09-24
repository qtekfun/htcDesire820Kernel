// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SCOPED_OBSERVER_H_
#define BASE_SCOPED_OBSERVER_H_

#include <algorithm>
#include <vector>

#include "base/basictypes.h"

template <class Source, class Observer>
class ScopedObserver {
 public:
  explicit ScopedObserver(Observer* observer) : observer_(observer) {}

  ~ScopedObserver() {
    for (size_t i = 0; i < sources_.size(); ++i)
      sources_[i]->RemoveObserver(observer_);
  }

  
  void Add(Source* source) {
    sources_.push_back(source);
    source->AddObserver(observer_);
  }

  
  void Remove(Source* source) {
    sources_.erase(std::find(sources_.begin(), sources_.end(), source));
    source->RemoveObserver(observer_);
  }

  bool IsObserving(Source* source) const {
    for (size_t i = 0; i < sources_.size(); ++i) {
      if (sources_[i] == source)
        return true;
    }
    return false;
  }

 private:
  Observer* observer_;

  std::vector<Source*> sources_;

  DISALLOW_COPY_AND_ASSIGN(ScopedObserver);
};

#endif  
