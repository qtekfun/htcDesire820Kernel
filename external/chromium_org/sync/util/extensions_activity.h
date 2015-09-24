// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_UTIL_EXTENSIONS_ACTIVITY_H_
#define SYNC_UTIL_EXTENSIONS_ACTIVITY_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "sync/base/sync_export.h"

namespace syncer {

class SYNC_EXPORT ExtensionsActivity
    : public base::RefCountedThreadSafe<ExtensionsActivity> {
 public:
  
  struct SYNC_EXPORT Record {
    Record();
    ~Record();

    
    
    std::string extension_id;

    
    
    uint32 bookmark_write_count;
  };

  typedef std::map<std::string, Record> Records;

  ExtensionsActivity();

  
  
  
  void GetAndClearRecords(Records* buffer);

  
  
  void PutRecords(const Records& records);

  
  void UpdateRecord(const std::string& extension_id);

 private:
  friend class base::RefCountedThreadSafe<ExtensionsActivity>;
  ~ExtensionsActivity();

  Records records_;
  mutable base::Lock records_lock_;
};

}  

#endif  
