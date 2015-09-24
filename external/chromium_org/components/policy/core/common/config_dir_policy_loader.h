// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CONFIG_DIR_POLICY_LOADER_H_
#define COMPONENTS_POLICY_CORE_COMMON_CONFIG_DIR_POLICY_LOADER_H_

#include "base/files/file_path.h"
#include "base/files/file_path_watcher.h"
#include "components/policy/core/common/async_policy_loader.h"
#include "components/policy/core/common/policy_types.h"
#include "components/policy/policy_export.h"

namespace base {
class Value;
}

namespace policy {

class POLICY_EXPORT ConfigDirPolicyLoader : public AsyncPolicyLoader {
 public:
  ConfigDirPolicyLoader(scoped_refptr<base::SequencedTaskRunner> task_runner,
                        const base::FilePath& config_dir,
                        PolicyScope scope);
  virtual ~ConfigDirPolicyLoader();

  
  virtual void InitOnBackgroundThread() OVERRIDE;
  virtual scoped_ptr<PolicyBundle> Load() OVERRIDE;
  virtual base::Time LastModificationTime() OVERRIDE;

 private:
  
  void LoadFromPath(const base::FilePath& path,
                    PolicyLevel level,
                    PolicyBundle* bundle);

  
  void Merge3rdPartyPolicy(const base::Value* policies,
                           PolicyLevel level,
                           PolicyBundle* bundle);

  
  void OnFileUpdated(const base::FilePath& path, bool error);

  
  base::FilePath config_dir_;

  
  PolicyScope scope_;

  
  
  base::FilePathWatcher mandatory_watcher_;
  base::FilePathWatcher recommended_watcher_;

  DISALLOW_COPY_AND_ASSIGN(ConfigDirPolicyLoader);
};

}  

#endif  
