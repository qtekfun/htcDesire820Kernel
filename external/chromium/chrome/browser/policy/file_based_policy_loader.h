// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_FILE_BASED_POLICY_LOADER_H_
#define CHROME_BROWSER_POLICY_FILE_BASED_POLICY_LOADER_H_
#pragma once

#include "base/files/file_path_watcher.h"
#include "chrome/browser/policy/asynchronous_policy_loader.h"
#include "chrome/browser/policy/file_based_policy_provider.h"

namespace policy {

class FileBasedPolicyLoader : public AsynchronousPolicyLoader {
 public:
  FileBasedPolicyLoader(
      FileBasedPolicyProvider::ProviderDelegate* provider_delegate);

  
  virtual void Reload();

  void OnFilePathChanged(const FilePath& path);
  void OnFilePathError(const FilePath& path);

 protected:
  
  
  friend class base::RefCountedThreadSafe<AsynchronousPolicyLoader>;
  virtual ~FileBasedPolicyLoader();

  const FilePath& config_file_path() { return config_file_path_; }

  

  
  
  virtual void InitOnFileThread();
  virtual void StopOnFileThread();

 private:
  
  
  
  bool IsSafeToReloadPolicy(const base::Time& now, base::TimeDelta* delay);

  
  const FilePath config_file_path_;

  
  
  
  
  
  scoped_ptr<base::files::FilePathWatcher> watcher_;

  
  const base::TimeDelta settle_interval_;

  
  base::Time last_modification_file_;

  
  
  
  
  base::Time last_modification_clock_;

  DISALLOW_COPY_AND_ASSIGN(FileBasedPolicyLoader);
};

}  

#endif  
