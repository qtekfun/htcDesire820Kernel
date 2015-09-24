// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_FILE_BASED_POLICY_PROVIDER_H_
#define CHROME_BROWSER_POLICY_FILE_BASED_POLICY_PROVIDER_H_
#pragma once

#include "base/file_path.h"
#include "base/time.h"
#include "chrome/browser/policy/asynchronous_policy_provider.h"

namespace policy {

class FileBasedPolicyProvider : public AsynchronousPolicyProvider {
 public:

  
  class ProviderDelegate : public AsynchronousPolicyProvider::Delegate {
   public:
    explicit ProviderDelegate(const FilePath& config_file_path);
    virtual ~ProviderDelegate();

    
    virtual DictionaryValue* Load() = 0;

    
    
    
    virtual base::Time GetLastModification() = 0;

    const FilePath& config_file_path() { return config_file_path_; }

   private:
    const FilePath config_file_path_;

    DISALLOW_COPY_AND_ASSIGN(ProviderDelegate);
  };

  
  FileBasedPolicyProvider(const PolicyDefinitionList* policy_list,
                          ProviderDelegate* delegate);
  virtual ~FileBasedPolicyProvider() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(FileBasedPolicyProvider);
};

}  

#endif  
