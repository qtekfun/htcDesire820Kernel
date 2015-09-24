// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_USER_POLICY_TOKEN_LOADER_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_USER_POLICY_TOKEN_LOADER_H_

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"

namespace base {
class SequencedTaskRunner;
}

namespace policy {

class UserPolicyTokenLoader
    : public base::RefCountedThreadSafe<UserPolicyTokenLoader> {
 public:
  
  class Delegate {
   public:
    virtual ~Delegate();
    virtual void OnTokenLoaded(const std::string& token,
                               const std::string& device_id) = 0;
  };

  UserPolicyTokenLoader(
      const base::WeakPtr<Delegate>& delegate,
      const base::FilePath& cache_file,
      scoped_refptr<base::SequencedTaskRunner> background_task_runner);

  
  
  void Load();

 private:
  friend class base::RefCountedThreadSafe<UserPolicyTokenLoader>;
  ~UserPolicyTokenLoader();

  void LoadOnBackgroundThread();
  void NotifyOnOriginThread(const std::string& token,
                        const std::string& device_id);

  const base::WeakPtr<Delegate> delegate_;
  const base::FilePath cache_file_;
  scoped_refptr<base::SequencedTaskRunner> origin_task_runner_;
  scoped_refptr<base::SequencedTaskRunner> background_task_runner_;

  DISALLOW_COPY_AND_ASSIGN(UserPolicyTokenLoader);
};

}  

#endif  
