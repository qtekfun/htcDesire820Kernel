// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_PUBLIC_CONSUMER_BASE_SUPPORTS_USER_DATA_H_
#define IOS_PUBLIC_CONSUMER_BASE_SUPPORTS_USER_DATA_H_

namespace ios {

class SupportsUserDataInternal;

class SupportsUserData {
 public:
  SupportsUserData();

  
  
  
  class Data {
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
  
  SupportsUserDataInternal* internal_helper_;
};

}  

#endif  
