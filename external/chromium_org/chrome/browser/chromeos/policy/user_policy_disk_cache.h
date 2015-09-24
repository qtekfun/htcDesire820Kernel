// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_USER_POLICY_DISK_CACHE_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_USER_POLICY_DISK_CACHE_H_

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"

namespace base {
class SequencedTaskRunner;
}

namespace enterprise_management {
class CachedCloudPolicyResponse;
}

namespace policy {

class UserPolicyDiskCache
    : public base::RefCountedThreadSafe<UserPolicyDiskCache> {
 public:
  
  enum LoadResult {
    
    LOAD_RESULT_SUCCESS,
    
    LOAD_RESULT_NOT_FOUND,
    
    LOAD_RESULT_READ_ERROR,
    
    LOAD_RESULT_PARSE_ERROR,
  };

  
  class Delegate {
   public:
    virtual ~Delegate();
    virtual void OnDiskCacheLoaded(
        LoadResult result,
        const enterprise_management::CachedCloudPolicyResponse& policy) = 0;
  };

  UserPolicyDiskCache(
      const base::WeakPtr<Delegate>& delegate,
      const base::FilePath& backing_file_path,
      scoped_refptr<base::SequencedTaskRunner> background_task_runner);

  
  
  
  void Load();

  
  void Store(const enterprise_management::CachedCloudPolicyResponse& policy);

 private:
  friend class base::RefCountedThreadSafe<UserPolicyDiskCache>;
  ~UserPolicyDiskCache();

  
  void LoadOnFileThread();

  
  void LoadDone(LoadResult result,
                const enterprise_management::CachedCloudPolicyResponse& policy);

  
  void ReportResultOnUIThread(
      LoadResult result,
      const enterprise_management::CachedCloudPolicyResponse& policy);

  
  void StoreOnFileThread(
      const enterprise_management::CachedCloudPolicyResponse& policy);

  base::WeakPtr<Delegate> delegate_;
  const base::FilePath backing_file_path_;
  scoped_refptr<base::SequencedTaskRunner> origin_task_runner_;
  scoped_refptr<base::SequencedTaskRunner> background_task_runner_;

  DISALLOW_COPY_AND_ASSIGN(UserPolicyDiskCache);
};

}  

#endif  
