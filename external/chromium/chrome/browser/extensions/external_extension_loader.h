// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTERNAL_EXTENSION_LOADER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTERNAL_EXTENSION_LOADER_H_
#pragma once

#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"

class DictionaryValue;
class ExternalExtensionProviderImpl;

class ExternalExtensionLoader
    : public base::RefCountedThreadSafe<ExternalExtensionLoader> {
 public:
  explicit ExternalExtensionLoader();

  
  void Init(ExternalExtensionProviderImpl* owner);

  
  void OwnerShutdown();

  
  
  
  
  
  virtual void StartLoading() = 0;

  
  
  
  
  
  virtual const FilePath GetBaseCrxFilePath();

 protected:
  virtual ~ExternalExtensionLoader();

  
  void LoadFinished();

  
  
  // if this value is written on another thread than the UI, then it should
  // only be written in a task that was posted from |StartLoading|. After that,
  
  
  
  scoped_ptr<DictionaryValue> prefs_;

 private:
  friend class base::RefCountedThreadSafe<ExternalExtensionLoader>;

  ExternalExtensionProviderImpl* owner_;  

  
  
  bool running_;

  DISALLOW_COPY_AND_ASSIGN(ExternalExtensionLoader);
};

#endif  
