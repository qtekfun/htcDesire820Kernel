// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_STORAGE_STORAGE_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_STORAGE_STORAGE_API_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/extensions/api/storage/settings_namespace.h"
#include "chrome/browser/extensions/api/storage/settings_observer.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/browser/value_store/value_store.h"

namespace extensions {

class SettingsFunction : public ChromeAsyncExtensionFunction {
 protected:
  SettingsFunction();
  virtual ~SettingsFunction();

  
  virtual bool ShouldSkipQuotaLimiting() const OVERRIDE;
  virtual bool RunImpl() OVERRIDE;

  
  
  
  
  virtual bool RunWithStorage(ValueStore* storage) = 0;

  
  
  bool UseReadResult(ValueStore::ReadResult result);

  
  
  
  bool UseWriteResult(ValueStore::WriteResult result);

 private:
  
  
  void AsyncRunWithStorage(ValueStore* storage);

  
  
  settings_namespace::Namespace settings_namespace_;

  
  scoped_refptr<SettingsObserverList> observers_;
};

class StorageStorageAreaGetFunction : public SettingsFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("storage.get", STORAGE_GET)

 protected:
  virtual ~StorageStorageAreaGetFunction() {}

  
  virtual bool RunWithStorage(ValueStore* storage) OVERRIDE;
};

class StorageStorageAreaSetFunction : public SettingsFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("storage.set", STORAGE_SET)

 protected:
  virtual ~StorageStorageAreaSetFunction() {}

  
  virtual bool RunWithStorage(ValueStore* storage) OVERRIDE;

  
  virtual void GetQuotaLimitHeuristics(
      QuotaLimitHeuristics* heuristics) const OVERRIDE;
};

class StorageStorageAreaRemoveFunction : public SettingsFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("storage.remove", STORAGE_REMOVE)

 protected:
  virtual ~StorageStorageAreaRemoveFunction() {}

  
  virtual bool RunWithStorage(ValueStore* storage) OVERRIDE;

  
  virtual void GetQuotaLimitHeuristics(
      QuotaLimitHeuristics* heuristics) const OVERRIDE;
};

class StorageStorageAreaClearFunction : public SettingsFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("storage.clear", STORAGE_CLEAR)

 protected:
  virtual ~StorageStorageAreaClearFunction() {}

  
  virtual bool RunWithStorage(ValueStore* storage) OVERRIDE;

  
  virtual void GetQuotaLimitHeuristics(
      QuotaLimitHeuristics* heuristics) const OVERRIDE;
};

class StorageStorageAreaGetBytesInUseFunction : public SettingsFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("storage.getBytesInUse", STORAGE_GETBYTESINUSE)

 protected:
  virtual ~StorageStorageAreaGetBytesInUseFunction() {}

  
  virtual bool RunWithStorage(ValueStore* storage) OVERRIDE;
};

}  

#endif  
