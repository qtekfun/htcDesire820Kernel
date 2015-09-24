// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_POLICY_LOADER_WIN_H_
#define COMPONENTS_POLICY_CORE_COMMON_POLICY_LOADER_WIN_H_

#include <windows.h>
#include <userenv.h>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "base/synchronization/waitable_event.h"
#include "base/win/object_watcher.h"
#include "components/policy/core/common/async_policy_loader.h"
#include "components/policy/core/common/policy_types.h"
#include "components/policy/policy_export.h"

namespace base {
class SequencedTaskRunner;
}

namespace policy {

class AppliedGPOListProvider;
class PolicyLoadStatusSample;
class PolicyMap;
class RegistryDict;

class POLICY_EXPORT AppliedGPOListProvider {
 public:
  virtual ~AppliedGPOListProvider() {}
  virtual DWORD GetAppliedGPOList(DWORD flags,
                                  LPCTSTR machine_name,
                                  PSID sid_user,
                                  GUID* extension_guid,
                                  PGROUP_POLICY_OBJECT* gpo_list) = 0;
  virtual BOOL FreeGPOList(PGROUP_POLICY_OBJECT gpo_list) = 0;
};

class POLICY_EXPORT PolicyLoaderWin
    : public AsyncPolicyLoader,
      public base::win::ObjectWatcher::Delegate {
 public:
  
  static const base::FilePath::CharType kPRegFileName[];

  PolicyLoaderWin(scoped_refptr<base::SequencedTaskRunner> task_runner,
                  const string16& chrome_policy_key,
                  AppliedGPOListProvider* gpo_provider);
  virtual ~PolicyLoaderWin();

  
  static scoped_ptr<PolicyLoaderWin> Create(
      scoped_refptr<base::SequencedTaskRunner> task_runner,
      const string16& chrome_policy_key);

  
  virtual void InitOnBackgroundThread() OVERRIDE;
  virtual scoped_ptr<PolicyBundle> Load() OVERRIDE;

 private:
  
  
  bool ReadPRegFile(const base::FilePath& preg_file,
                    RegistryDict* policy,
                    PolicyLoadStatusSample *status);

  
  
  
  
  bool LoadGPOPolicy(PolicyScope scope,
                     PGROUP_POLICY_OBJECT policy_object_list,
                     RegistryDict* policy,
                     PolicyLoadStatusSample *status);

  
  
  
  bool ReadPolicyFromGPO(PolicyScope scope,
                         RegistryDict* policy,
                         PolicyLoadStatusSample *status);

  
  
  void LoadChromePolicy(const RegistryDict* gpo_dict,
                        PolicyLevel level,
                        PolicyScope scope,
                        PolicyMap* chrome_policy_map);

  
  void Load3rdPartyPolicy(const RegistryDict* gpo_dict,
                          PolicyScope scope,
                          PolicyBundle* bundle);

  
  void SetupWatches();

  
  virtual void OnObjectSignaled(HANDLE object) OVERRIDE;

  bool is_initialized_;
  const string16 chrome_policy_key_;
  class AppliedGPOListProvider* gpo_provider_;

  base::WaitableEvent user_policy_changed_event_;
  base::WaitableEvent machine_policy_changed_event_;
  base::win::ObjectWatcher user_policy_watcher_;
  base::win::ObjectWatcher machine_policy_watcher_;
  bool user_policy_watcher_failed_;
  bool machine_policy_watcher_failed_;

  DISALLOW_COPY_AND_ASSIGN(PolicyLoaderWin);
};

}  

#endif  
