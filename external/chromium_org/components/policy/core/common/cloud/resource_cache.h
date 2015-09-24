// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_RESOURCE_CACHE_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_RESOURCE_CACHE_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "components/policy/policy_export.h"

namespace base {
class SequencedTaskRunner;
}

namespace policy {

class POLICY_EXPORT ResourceCache {
 public:
  explicit ResourceCache(const base::FilePath& cache_path,
                         scoped_refptr<base::SequencedTaskRunner> task_runner);
  virtual ~ResourceCache();

  
  
  bool Store(const std::string& key,
             const std::string& subkey,
             const std::string& data);

  
  
  
  bool Load(const std::string& key,
            const std::string& subkey,
            std::string* data);

  
  void LoadAllSubkeys(const std::string& key,
                      std::map<std::string, std::string>* contents);

  
  void Delete(const std::string& key, const std::string& subkey);

  
  void Clear(const std::string& key);

  
  typedef base::Callback<bool(const std::string&)> SubkeyFilter;
  void FilterSubkeys(const std::string& key, const SubkeyFilter& filter);

  
  void PurgeOtherKeys(const std::set<std::string>& keys_to_keep);

  
  void PurgeOtherSubkeys(const std::string& key,
                         const std::set<std::string>& subkeys_to_keep);

 private:
  
  
  
  bool VerifyKeyPath(const std::string& key,
                     bool allow_create,
                     base::FilePath* path);

  
  
  
  
  bool VerifyKeyPathAndGetSubkeyPath(const std::string& key,
                                     bool allow_create_key,
                                     const std::string& subkey,
                                     base::FilePath* subkey_path);

  base::FilePath cache_dir_;

  
  scoped_refptr<base::SequencedTaskRunner> task_runner_;

  DISALLOW_COPY_AND_ASSIGN(ResourceCache);
};

}  

#endif  
