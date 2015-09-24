// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRARIES_NACL_IO_TEST_FAKE_RESOURCE_MANAGER_H_
#define LIBRARIES_NACL_IO_TEST_FAKE_RESOURCE_MANAGER_H_

#include <map>

#include <ppapi/c/pp_resource.h>

#include "sdk_util/atomicops.h"
#include "sdk_util/macros.h"
#include "sdk_util/simple_lock.h"

class FakeResource;
class FakeResourceTracker;

class FakeResourceManager {
 public:
  FakeResourceManager();
  ~FakeResourceManager();

  PP_Resource Create(FakeResource* resource,
                     const char* classname,
                     const char* file,
                     int line);
  void AddRef(PP_Resource handle);
  void Release(PP_Resource handle);
  template <typename T>
  T* Get(PP_Resource handle);

 private:
  FakeResourceTracker* Get(PP_Resource handle);

  typedef std::map<PP_Resource, FakeResourceTracker*> ResourceMap;
  PP_Resource next_handle_;
  ResourceMap resource_map_;
  sdk_util::SimpleLock lock_;  

  DISALLOW_COPY_AND_ASSIGN(FakeResourceManager);
};

class FakeResourceTracker {
 public:
  FakeResourceTracker(FakeResource* resource,
                      const char* classname,
                      const char* file,
                      int line);
  ~FakeResourceTracker();

  void AddRef() { sdk_util::AtomicAddFetch(&ref_count_, 1); }
  void Release() { sdk_util::AtomicAddFetch(&ref_count_, -1); }
  int32_t ref_count() const { return ref_count_; }

  
  
  FakeResource* Pass() {
    FakeResource* result = resource_;
    resource_ = NULL;
    return result;
  }

  template <typename T>
  T* resource() {
    if (!CheckType(T::classname()))
      return NULL;

    return static_cast<T*>(resource_);
  }

  FakeResource* resource() { return resource_; }

  const char* classname() const { return classname_; }
  const char* file() const { return file_; }
  int line() const { return line_; }

 private:
  bool CheckType(const char* classname) const;

  FakeResource* resource_;  
  const char* classname_;   
  const char* file_;        
  int line_;
  int32_t ref_count_;

  DISALLOW_COPY_AND_ASSIGN(FakeResourceTracker);
};

class FakeResource {
 public:
  FakeResource() {}
  
  
  virtual ~FakeResource() {}
  
  virtual void Destroy() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(FakeResource);
};

template <typename T>
inline T* FakeResourceManager::Get(PP_Resource handle) {
  return Get(handle)->resource<T>();
}

#define CREATE_RESOURCE(MANAGER, TYPE, RESOURCE) \
  (MANAGER)->Create((RESOURCE), #TYPE, __FILE__, __LINE__)

#endif  
