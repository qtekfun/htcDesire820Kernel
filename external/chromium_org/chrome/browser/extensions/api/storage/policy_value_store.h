// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_STORAGE_POLICY_VALUE_STORE_H_
#define CHROME_BROWSER_EXTENSIONS_API_STORAGE_POLICY_VALUE_STORE_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/api/storage/settings_observer.h"
#include "chrome/browser/value_store/value_store.h"

namespace policy {
class PolicyMap;
}

namespace extensions {

class PolicyValueStore : public ValueStore {
 public:
  PolicyValueStore(const std::string& extension_id,
                   const scoped_refptr<SettingsObserverList>& observers,
                   scoped_ptr<ValueStore> delegate);
  virtual ~PolicyValueStore();

  
  
  void SetCurrentPolicy(const policy::PolicyMap& policy);

  
  void DeleteStorage();

  
  virtual size_t GetBytesInUse(const std::string& key) OVERRIDE;
  virtual size_t GetBytesInUse(const std::vector<std::string>& keys) OVERRIDE;
  virtual size_t GetBytesInUse() OVERRIDE;
  virtual ReadResult Get(const std::string& key) OVERRIDE;
  virtual ReadResult Get(const std::vector<std::string>& keys) OVERRIDE;
  virtual ReadResult Get() OVERRIDE;
  virtual WriteResult Set(
      WriteOptions options,
      const std::string& key,
      const base::Value& value) OVERRIDE;
  virtual WriteResult Set(
      WriteOptions options, const base::DictionaryValue& values) OVERRIDE;
  virtual WriteResult Remove(const std::string& key) OVERRIDE;
  virtual WriteResult Remove(const std::vector<std::string>& keys) OVERRIDE;
  virtual WriteResult Clear() OVERRIDE;

  
  ValueStore* delegate() { return delegate_.get(); }

 private:
  std::string extension_id_;
  scoped_refptr<SettingsObserverList> observers_;
  scoped_ptr<ValueStore> delegate_;

  DISALLOW_COPY_AND_ASSIGN(PolicyValueStore);
};

}  

#endif  
