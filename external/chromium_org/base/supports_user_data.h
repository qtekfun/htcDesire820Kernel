// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SUPPORTS_USER_DATA_H_
#define BASE_SUPPORTS_USER_DATA_H_

#include <map>

#include "base/base_export.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/threading/thread_checker.h"

namespace base {

class BASE_EXPORT SupportsUserData {
 public:
  SupportsUserData();

  
  
  
  class BASE_EXPORT Data {
   public:
    virtual ~Data() {}
  };

  
  
  
  
  Data* GetUserData(const void* key) const;
  void SetUserData(const void* key, Data* data);
  void RemoveUserData(const void* key);

  
  
  
  
  void DetachUserDataThread();

 protected:
  virtual ~SupportsUserData();

 private:
  typedef std::map<const void*, linked_ptr<Data> > DataMap;

  
  DataMap user_data_;
  
  ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(SupportsUserData);
};

template <typename T>
class UserDataAdapter : public base::SupportsUserData::Data {
 public:
  static T* Get(SupportsUserData* supports_user_data, const void* key) {
    UserDataAdapter* data =
      static_cast<UserDataAdapter*>(supports_user_data->GetUserData(key));
    return data ? static_cast<T*>(data->object_.get()) : NULL;
  }

  UserDataAdapter(T* object) : object_(object) {}
  T* release() { return object_.release(); }

 private:
  scoped_refptr<T> object_;

  DISALLOW_COPY_AND_ASSIGN(UserDataAdapter);
};

}  

#endif  
