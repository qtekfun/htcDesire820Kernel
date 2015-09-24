// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_STORAGE_VALUE_STORE_CACHE_H_
#define CHROME_BROWSER_EXTENSIONS_API_STORAGE_VALUE_STORE_CACHE_H_

#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"

class ValueStore;

namespace extensions {

class Extension;

class ValueStoreCache {
 public:
  typedef base::Callback<void(ValueStore*)> StorageCallback;

  
  virtual ~ValueStoreCache();

  
  
  
  
  virtual void ShutdownOnUI();

  
  
  
  
  
  
  virtual void RunWithValueStoreForExtension(
      const StorageCallback& callback,
      scoped_refptr<const Extension> extension) = 0;

  
  virtual void DeleteStorageSoon(const std::string& extension_id) = 0;
};

}  

#endif  
