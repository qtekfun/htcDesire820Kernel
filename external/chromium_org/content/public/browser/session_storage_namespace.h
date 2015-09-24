// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_SESSION_STORAGE_NAMESPACE_H_
#define CONTENT_PUBLIC_BROWSER_SESSION_STORAGE_NAMESPACE_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"

namespace content {

class SessionStorageNamespace
    : public base::RefCountedThreadSafe<SessionStorageNamespace> {
 public:
  
  
  virtual int64 id() const = 0;

  
  
  virtual const std::string& persistent_id() const = 0;

  
  
  virtual void SetShouldPersist(bool should_persist) = 0;

  virtual bool should_persist() const = 0;

  
  
  
  
  virtual void AddTransactionLogProcessId(int process_id) = 0;

  
  
  virtual void RemoveTransactionLogProcessId(int process_id) = 0;

  
  
  virtual SessionStorageNamespace* CreateAlias() = 0;

  enum MergeResult {
    MERGE_RESULT_NAMESPACE_NOT_FOUND,
    MERGE_RESULT_NAMESPACE_NOT_ALIAS,
    MERGE_RESULT_NOT_LOGGING,
    MERGE_RESULT_NO_TRANSACTIONS,
    MERGE_RESULT_TOO_MANY_TRANSACTIONS,
    MERGE_RESULT_NOT_MERGEABLE,
    MERGE_RESULT_MERGEABLE,
    MERGE_RESULT_MAX_VALUE
  };

  typedef base::Callback<void(MergeResult)> MergeResultCallback;

  
  
  
  
  
  
  virtual void Merge(bool actually_merge,
                     int process_id,
                     SessionStorageNamespace* other,
                     const MergeResultCallback& callback) = 0;

  
  
  virtual bool IsAliasOf(SessionStorageNamespace* other) = 0;

 protected:
  friend class base::RefCountedThreadSafe<SessionStorageNamespace>;
  virtual ~SessionStorageNamespace() {}
};

}  

#endif  
