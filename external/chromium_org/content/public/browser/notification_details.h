// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_PUBLIC_BROWSER_NOTIFICATION_DETAILS_H_
#define CONTENT_PUBLIC_BROWSER_NOTIFICATION_DETAILS_H_

#include "base/basictypes.h"
#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT NotificationDetails {
 public:
  NotificationDetails() : ptr_(NULL) {}
  NotificationDetails(const NotificationDetails& other) : ptr_(other.ptr_) {}
  ~NotificationDetails() {}

  
  
  
  uintptr_t map_key() const { return reinterpret_cast<uintptr_t>(ptr_); }

  bool operator!=(const NotificationDetails& other) const {
    return ptr_ != other.ptr_;
  }

  bool operator==(const NotificationDetails& other) const {
    return ptr_ == other.ptr_;
  }

 protected:
   explicit NotificationDetails(const void* ptr) : ptr_(ptr) {}

  
  
  const void* ptr_;
};

template <class T>
class Details : public NotificationDetails {
 public:
  
  Details(T* ptr) : NotificationDetails(ptr) {}  
  Details(const NotificationDetails& other)      
    : NotificationDetails(other) {}

  T* operator->() const { return ptr(); }
  
  T* ptr() const { return static_cast<T*>(const_cast<void*>(ptr_)); }
};

}  

#endif  
