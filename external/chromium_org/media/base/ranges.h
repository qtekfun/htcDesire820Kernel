// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_RANGES_H_
#define MEDIA_BASE_RANGES_H_

#include <algorithm>
#include <ostream>
#include <vector>

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/time/time.h"
#include "media/base/media_export.h"

namespace media {

template<class T>  
class Ranges {
 public:
  

  
  
  size_t Add(T start, T end);

  
  size_t size() const;

  
  T start(int i) const;
  T end(int i) const;

  
  void clear();

  
  Ranges<T> IntersectionWith(const Ranges<T>& other) const;

 private:
  
  void DCheckLT(const T& lhs, const T& rhs) const;

  
  std::vector<std::pair<T, T> > ranges_;
};


template<class T>
size_t Ranges<T>::Add(T start, T end) {
  if (start == end)  
    return ranges_.size();

  DCheckLT(start, end);
  size_t i;
  
  
  for (i = 0; i < ranges_.size() && ranges_[i].second < start; ++i) {
    
  }

  
  
  if (i == ranges_.size()) {
    ranges_.push_back(std::make_pair(start, end));
    return ranges_.size();
  }

  
  
  if (end < ranges_[i].first) {
    ranges_.insert(ranges_.begin() + i, std::make_pair(start, end));
    return ranges_.size();
  }

  
  

  
  if (start < ranges_[i].first)
    ranges_[i].first = start;
  if (ranges_[i].second < end)
    ranges_[i].second = end;

  
  
  
  
  
  while ((i + 1) < ranges_.size() &&
         ranges_[i + 1].first <= ranges_[i].second) {
    ranges_[i].second = std::max(ranges_[i].second, ranges_[i + 1].second);
    ranges_.erase(ranges_.begin() + i + 1);
  }

  return ranges_.size();
}

template<>
MEDIA_EXPORT void
    Ranges<base::TimeDelta>::DCheckLT(const base::TimeDelta& lhs,
                                      const base::TimeDelta& rhs) const;

template<class T>
void Ranges<T>::DCheckLT(const T& lhs, const T& rhs) const {
  DCHECK_LT(lhs, rhs);
}

template<class T>
size_t Ranges<T>::size() const {
  return ranges_.size();
}

template<class T>
T Ranges<T>::start(int i) const {
  return ranges_[i].first;
}

template<class T>
T Ranges<T>::end(int i) const {
  return ranges_[i].second;
}

template<class T>
void Ranges<T>::clear() {
  ranges_.clear();
}

template<class T>
Ranges<T> Ranges<T>::IntersectionWith(const Ranges<T>& other) const {
  Ranges<T> result;

  size_t i = 0;
  size_t j = 0;

  while (i < size() && j < other.size()) {
    T max_start = std::max(start(i), other.start(j));
    T min_end = std::min(end(i), other.end(j));

    
    if (max_start < min_end)
      result.Add(max_start, min_end);

    if (end(i) < other.end(j))
      ++i;
    else
      ++j;
  }

  return result;
}

}  

#endif  
