// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_INDEXED_DB_CONTEXT_H_
#define CONTENT_PUBLIC_BROWSER_INDEXED_DB_CONTEXT_H_

#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "content/public/browser/indexed_db_info.h"

namespace base {
class SequencedTaskRunner;
class TaskRunner;
}

namespace content {

class IndexedDBContext : public base::RefCountedThreadSafe<IndexedDBContext> {
 public:
  
  virtual base::TaskRunner* TaskRunner() const = 0;

  
  virtual std::vector<IndexedDBInfo> GetAllOriginsInfo() = 0;
  virtual int64 GetOriginDiskUsage(const GURL& origin_url) = 0;

  
  virtual void DeleteForOrigin(const GURL& origin_url) = 0;

  
  virtual base::FilePath GetFilePathForTesting(
      const std::string& origin_id) const = 0;
  
  virtual void SetTaskRunnerForTesting(
      base::SequencedTaskRunner* task_runner) = 0;

 protected:
  friend class base::RefCountedThreadSafe<IndexedDBContext>;
  virtual ~IndexedDBContext() {}
};

}  

#endif  
