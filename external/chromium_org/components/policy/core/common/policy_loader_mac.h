// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_POLICY_LOADER_MAC_H_
#define COMPONENTS_POLICY_CORE_COMMON_POLICY_LOADER_MAC_H_

#include <string>

#include <CoreFoundation/CoreFoundation.h>

#include "base/files/file_path.h"
#include "base/files/file_path_watcher.h"
#include "base/memory/ref_counted.h"
#include "components/policy/core/common/async_policy_loader.h"
#include "components/policy/core/common/policy_namespace.h"
#include "components/policy/policy_export.h"

class MacPreferences;

namespace base {
class SequencedTaskRunner;
class Value;
}  

namespace policy {

class PolicyBundle;
class PolicyMap;
class Schema;

class POLICY_EXPORT PolicyLoaderMac : public AsyncPolicyLoader {
 public:
  PolicyLoaderMac(scoped_refptr<base::SequencedTaskRunner> task_runner,
                  const base::FilePath& managed_policy_path,
                  MacPreferences* preferences);
  virtual ~PolicyLoaderMac();

  
  virtual void InitOnBackgroundThread() OVERRIDE;
  virtual scoped_ptr<PolicyBundle> Load() OVERRIDE;
  virtual base::Time LastModificationTime() OVERRIDE;

  
  
  
  
  static base::Value* CreateValueFromProperty(CFPropertyListRef property);

 private:
  
  void OnFileUpdated(const base::FilePath& path, bool error);

  
  
  void LoadPolicyForDomain(
      PolicyDomain domain,
      const std::string& domain_name,
      PolicyBundle* bundle);

  
  
  void LoadPolicyForComponent(const std::string& bundle_id_string,
                              const Schema& schema,
                              PolicyMap* policy);

  scoped_ptr<MacPreferences> preferences_;

  
  
  base::FilePath managed_policy_path_;

  
  base::FilePathWatcher watcher_;

  DISALLOW_COPY_AND_ASSIGN(PolicyLoaderMac);
};

}  

#endif  
