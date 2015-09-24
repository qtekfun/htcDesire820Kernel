// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOM_STORAGE_DOM_STORAGE_SESSION_H_
#define CONTENT_BROWSER_DOM_STORAGE_DOM_STORAGE_SESSION_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "content/browser/dom_storage/session_storage_namespace_impl.h"
#include "content/common/content_export.h"

namespace content {

class DOMStorageContextImpl;

class CONTENT_EXPORT DOMStorageSession
    : public base::RefCountedThreadSafe<DOMStorageSession> {
 public:
  
  explicit DOMStorageSession(DOMStorageContextImpl* context);

  
  
  DOMStorageSession(DOMStorageContextImpl* context,
                    const std::string& persistent_namespace_id);

  
  
  explicit DOMStorageSession(DOMStorageSession* master_dom_storage_session);

  int64 namespace_id() const { return namespace_id_; }
  const std::string& persistent_namespace_id() const {
    return persistent_namespace_id_;
  }
  void SetShouldPersist(bool should_persist);
  bool should_persist() const;
  bool IsFromContext(DOMStorageContextImpl* context);
  DOMStorageSession* Clone();

  
  
  static DOMStorageSession* CloneFrom(DOMStorageContextImpl* context,
                                      int64 namepace_id_to_clone);

  void AddTransactionLogProcessId(int process_id);
  void RemoveTransactionLogProcessId(int process_id);
  void Merge(bool actually_merge,
             int process_id,
             DOMStorageSession* other,
             const SessionStorageNamespace::MergeResultCallback& callback);

 private:
  friend class base::RefCountedThreadSafe<DOMStorageSession>;

  DOMStorageSession(DOMStorageContextImpl* context,
                    int64 namespace_id,
                    const std::string& persistent_namespace_id);
  ~DOMStorageSession();

  void ProcessMergeResult(
      bool actually_merge,
      const SessionStorageNamespace::MergeResultCallback& callback,
      const std::string& new_persistent_namespace_id,
      SessionStorageNamespace::MergeResult result);

  scoped_refptr<DOMStorageContextImpl> context_;
  int64 namespace_id_;
  std::string persistent_namespace_id_;
  bool should_persist_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(DOMStorageSession);
};

}  

#endif  
