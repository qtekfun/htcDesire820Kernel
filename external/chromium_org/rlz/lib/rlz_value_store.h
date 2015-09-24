// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RLZ_VALUE_STORE_H_
#define RLZ_VALUE_STORE_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "rlz/lib/rlz_enums.h"

#if defined(OS_WIN)
#include "rlz/win/lib/lib_mutex.h"
#endif

#if defined(OS_MACOSX)
#include "base/mac/scoped_nsautorelease_pool.h"
#endif


#include <string>
#include <vector>

namespace base {
class FilePath;
}

namespace rlz_lib {

class RlzValueStore {
 public:
  virtual ~RlzValueStore() {}

  enum AccessType { kReadAccess, kWriteAccess };
  virtual bool HasAccess(AccessType type) = 0;

  
  virtual bool WritePingTime(Product product, int64 time) = 0;
  virtual bool ReadPingTime(Product product, int64* time) = 0;
  virtual bool ClearPingTime(Product product) = 0;

  
  virtual bool WriteAccessPointRlz(AccessPoint access_point,
                                   const char* new_rlz) = 0;
  virtual bool ReadAccessPointRlz(AccessPoint access_point,
                                  char* rlz,  
                                  size_t rlz_size) = 0;
  virtual bool ClearAccessPointRlz(AccessPoint access_point) = 0;

  
  
  virtual bool AddProductEvent(Product product, const char* event_rlz) = 0;
  
  virtual bool ReadProductEvents(Product product,
                                 std::vector<std::string>* events) = 0;
  
  virtual bool ClearProductEvent(Product product, const char* event_rlz) = 0;
  
  virtual bool ClearAllProductEvents(Product product) = 0;

  
  
  virtual bool AddStatefulEvent(Product product, const char* event_rlz) = 0;
  
  virtual bool IsStatefulEvent(Product product, const char* event_rlz) = 0;
  
  virtual bool ClearAllStatefulEvents(Product product) = 0;

  
  
  
  virtual void CollectGarbage() = 0;
};

class ScopedRlzValueStoreLock {
 public:
  ScopedRlzValueStoreLock();
  ~ScopedRlzValueStoreLock();

  
  
  
  RlzValueStore* GetStore();

 private:
  scoped_ptr<RlzValueStore> store_;
#if defined(OS_WIN)
  LibMutex lock_;
#elif defined(OS_MACOSX)
  base::mac::ScopedNSAutoreleasePool autorelease_pool_;
#endif
};

#if defined(OS_POSIX)
namespace testing {
void SetRlzStoreDirectory(const base::FilePath& directory);

std::string RlzStoreFilenameStr();
}  
#endif  

}  

#endif  
