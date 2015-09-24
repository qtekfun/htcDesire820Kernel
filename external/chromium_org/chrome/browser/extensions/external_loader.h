// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTERNAL_LOADER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTERNAL_LOADER_H_

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
}

namespace extensions {
class ExternalProviderImpl;

class ExternalLoader : public base::RefCountedThreadSafe<ExternalLoader> {
 public:
  ExternalLoader();

  
  void Init(ExternalProviderImpl* owner);

  
  void OwnerShutdown();

  
  
  
  
  
  virtual void StartLoading() = 0;

  
  
  
  
  
  virtual const base::FilePath GetBaseCrxFilePath();

 protected:
  virtual ~ExternalLoader();

  
  virtual void LoadFinished();

  
  
  // if this value is written on another thread than the UI, then it should
  // only be written in a task that was posted from |StartLoading|. After that,
  
  
  
  scoped_ptr<base::DictionaryValue> prefs_;

 private:
  friend class base::RefCountedThreadSafe<ExternalLoader>;

  ExternalProviderImpl* owner_;  

  DISALLOW_COPY_AND_ASSIGN(ExternalLoader);
};

}  

#endif  
